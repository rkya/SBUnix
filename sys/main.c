#include <sys/defs.h>
#include <sys/gdt.h>
#include <sys/kprintf.h>
#include <sys/tarfs.h>
#include <sys/ahci.h>
#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/paging.h>
#include <sys/process.h>
#include <sys/timer.h>

#define INITIAL_STACK_SIZE 4096
uint8_t initial_stack[INITIAL_STACK_SIZE]__attribute__((aligned(16)));
uint32_t* loader_stack;
extern char kernmem, physbase;


void loop() {
  while(1) {
    kprintf("Reached the end of start function...\n");
    //p_remove_process(p_get_current_process());
    yield();
  }
}

void add_first_process() {
  pcb *p1 = p_get_new_process("First Process");
  void (*f_ptr3)() = &loop;
  p1->kstack[127] = (uint64_t)f_ptr3;
}

void start(uint32_t *modulep, void *physbase, void *physfree)
{
  //initialize a blank screen
  t_init_screen();
  kprintf("--------------------------------------------------------------------------------");
  kprintf("Booted up!!!\n");

  struct smap_t {
    uint64_t base, length;
    uint32_t type;
  }__attribute__((packed)) *smap;
  int index = 0;
  uint64_t end = 0;
  while(modulep[0] != 0x9001) modulep += modulep[1]+2;
  for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2*4); ++smap) {
    if (smap->type == 1 /* memory */ && smap->length != 0) {
      if(index == 2)
        break;
      index++;
      kprintf("Available Physical Memory [%p-%p]\n", smap->base, smap->base + smap->length);
      end = smap->base + smap->length;
    }
  }
  kprintf("physfree %p\n", (uint64_t)physfree);
  kprintf("tarfs in [%p:%p]\n", &_binary_tarfs_start, &_binary_tarfs_end);
  kprintf("--------------------------------------------------------------------------------");

  g_init_gdt();
  kprintf("Initialized GDT.\n");

  i_init_idt();
  kprintf("Initialized IDT.\n");

  /*t_init_timer();
  kprintf("Initialized Timer.\n");*/

  pml4 *pml4_t = p_init_paging((uint64_t)physbase, (uint64_t)physfree, end);
  kprintf("Initialized paging.\n");
  kprintf("pml4_t = %p\n", pml4_t);

  t_init_tarfs();
  kprintf("Initialized tarfs.\n");
  t_print_vfs();
  kprintf("\n");

  p_init_process();
  kprintf("Initialized processes.\n");
  add_first_process();
  pcb *sbush = p_get_new_process("sbush");
  kprintf("Created a new process: %s.\n", sbush->name);
  kprintf("active_queue_size = %d, current_process = %d, pid_allocator_index = %d.\n",
          active_queue_size, current_process, pid_allocator_index);

  /*pcb *p3 = p_get_new_process("p3");
  kprintf("Created a new process: %s.\n", p3->name);
  pcb *p4 = p_get_new_process("p4");
  kprintf("Created a new process: %s.\n", p4->name);
  pcb *p5 = p_get_new_process("p5");
  kprintf("Created a new process: %s.\n", p5->name);*/

	loop();
}

void boot(void)
{
  // note: function changes rsp, local stack variables can't be practically used
  register char *temp1, *temp2;

  for(temp2 = (char*)0xb8001; temp2 < (char*)0xb8000+160*25; temp2 += 2) *temp2 = 7 /* white */;
  __asm__ volatile (
    "cli;"
    "movq %%rsp, %0;"
    "movq %1, %%rsp;"
    :"=g"(loader_stack)
    :"r"(&initial_stack[INITIAL_STACK_SIZE])
  );

  start(
    (uint32_t*)((char*)(uint64_t)loader_stack[3] + (uint64_t)&kernmem - (uint64_t)&physbase),
    (uint64_t*)&physbase,
    (uint64_t*)(uint64_t)loader_stack[4]
  );
  for(
    temp1 = "!!!!! start() returned !!!!!", temp2 = (char*)0xb8000;
    *temp1;
    temp1 += 1, temp2 += 2
  ) *temp2 = *temp1;
  while(1) __asm__ volatile ("hlt");
}
