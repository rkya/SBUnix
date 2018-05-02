#include <sys/process.h>
#include <sys/kprintf.h>

#define HEX_ONE_THOUSAND 0x1000

void switch_to(pcb*, pcb*);
uint64_t get_new_process_no();
int add_to_active_queue(pcb *);
void sbush_function();
int test_sbush_main();
//int main(int argc, char *argv[], char *envp[]);


void sbush_function() {
  while (1) {
    kprintf("Inside sbush by process %s...\n", p_get_current_process()->name);
    //main(1, NULL, NULL);
    /*char **a = NULL;
    char **b = NULL;
    int c = 0;
    sbush_main(c, a, b);*/
    /*p_switch_to_user_mode();
    kprintf("In ring 3...\n");
    while (1);*/
    while (1);
    yield();
  }
}

void p_init_process() {
  active_queue_size = 0;
  current_process = 0;
  pid_allocator_index = 0;

  for(int i = 0; i < MAX_PROCESSES; i++) {
    active_queue[i] = NULL;
  }
}

void p_print_all_active_processes() {
  kprintf("Name | pid | ppid\n");
  for(int i = 0; i < MAX_PROCESSES; i++) {
    if(active_queue[i] != NULL && active_queue[i]->state == RUNNING) {
      kprintf("%s | %d | %d\n", active_queue[i]->name, active_queue[i]->pid, active_queue[i]->ppid);
    }
  }
}

uint64_t get_new_process_no() {
  int count = MAX_PROCESSES;
  if(active_queue_size >= MAX_PROCESSES) {
    return -1;
  }
  while(count-- >= 0) {
    if(active_queue[pid_allocator_index] != NULL && active_queue[pid_allocator_index]->state != ZOMBIE) {
      pid_allocator_index = (pid_allocator_index + 1) % MAX_PROCESSES;
      continue;
    }
    return pid_allocator_index++;
  }
  return -1;
}

int add_to_active_queue(pcb *new_pcb) {
  if(new_pcb->pid == -1) {
    return -1;
  }
  active_queue[new_pcb->pid] = new_pcb;
  active_queue_size++;
  new_pcb->state = RUNNING;
  return 1;
}

/***
 * Schedule the next active process in the queue. This scheduler considers every process of the same priority and
 * schedules the processes in a round-robin fashion.
 */
void schedule() {
  //int count = MAX_PROCESSES;
  int next_process = -1;
  for(int i = (current_process + 1) % active_queue_size; i < MAX_PROCESSES; i = (i + 1) % active_queue_size) {
    if(active_queue[i % active_queue_size]->state == RUNNING) {
      next_process = i % active_queue_size;
      break;
    }
  }
  if(next_process >= 0) {
    uint64_t temp_process = current_process;
    current_process = next_process;
    switch_to(active_queue[temp_process], active_queue[next_process]);
  } else {
    switch_to(active_queue[current_process], active_queue[current_process]);
  }
}

int p_remove_process(pcb *process) {
  int i;
  if(process->state == ZOMBIE) {
    return 1;
  }
  for(i = 0; i < MAX_PROCESSES; i++) {
    if(process == active_queue[i]) {
      process->state = ZOMBIE;
      active_queue_size--;
      return 1;
    }
  }
  return -1;
}

int p_remove_process_by_id(pid_t process_id) {
  int i;

  for(i = 0; i < MAX_PROCESSES; i++) {
    if(active_queue[i] != NULL && process_id == active_queue[i]->pid) {
      if(active_queue[i]->state == ZOMBIE) {
        return 1;
      } else {
        active_queue[i]->state = ZOMBIE;
        active_queue_size--;
        return 1;
      }
    }
  }
  return -1;
}

void yield() {
  schedule();
}

pcb *p_get_new_process(char *filename) {
  //create new process
  pcb *new_pcb = (pcb *) kmalloc (sizeof(pcb));

  new_pcb->pid = (pid_t)get_new_process_no();
  if(add_to_active_queue(new_pcb) == -1) {
    //TODO: free the used memeory
    return NULL;
  }

  //assign basic details needed for process execution
  strcpy(new_pcb->current_directory, "/");
  //new_pcb->mm_struct_ptr = (mm_struct *) ((char *)(new_pcb + 1));
  new_pcb->mm_struct_ptr = (mm_struct *) kmalloc(sizeof(mm_struct));
  new_pcb->mm_struct_ptr->map_count = 0;
  new_pcb->mm_struct_ptr->vma_head = NULL;
  new_pcb->mm_struct_ptr->vma_tail = NULL;
  new_pcb->ppid = (pid_t)current_process;
  strcpy(new_pcb->name, filename);

  //load the binary in the kstack and make it ready for execution

  //  void (*f_ptr3)() = &sbush_function;
  int (*f_ptr3)() = &test_sbush_main;
  /*char **temp_a = NULL;
  char **temp_b = NULL;*/
//  int (*f_ptr3)(int, char **, char **) = &main;
  new_pcb->kstack[127] = (uint64_t)f_ptr3;
  new_pcb->rsp = (uint64_t)&new_pcb->kstack[112];


  //assign file descriptors for input/output and error
  strcpy(new_pcb->fd_array[0].name, "stdin");
  strcpy(new_pcb->fd_array[1].name, "stdout");
  strcpy(new_pcb->fd_array[2].name, "stderr");
  new_pcb->fd_array[0].live = 1;
  new_pcb->fd_array[1].live = 1;
  new_pcb->fd_array[2].live = 1;
  new_pcb->fd_array[0].index = 0;
  new_pcb->fd_array[1].index = 1;
  new_pcb->fd_array[2].index = 2;
  new_pcb->fd_array[0].buffer[0] = '\0';
  new_pcb->fd_array[1].buffer[0] = '\0';
  new_pcb->fd_array[2].buffer[0] = '\0';
  new_pcb->fd_array[0].position = 0;
  new_pcb->fd_array[1].position = 0;
  new_pcb->fd_array[2].position = 0;
  new_pcb->fd_array_size = 3;

  return new_pcb;
}

pcb *p_get_current_process() {
  return active_queue[current_process];
}

pcb *p_get_process_by_id(int pid) {
  return active_queue[pid];
}

pcb *p_get_parent_process(pcb *process) {
  for(int i = 0; i < MAX_PROCESSES; i++) {
    if(active_queue[i] != NULL && !strcmp(active_queue[i]->name, process->name)) {
      return active_queue[i];
    }
  }
  return NULL;
}

/***
 * Returns if a process has atleast 1 child
 * @param process pcb pointer of the process
 * @return 1 if it has atleast 1 child, 0 otherwise
 */
int p_has_child_processes(pcb *process) {
  for(int i = 1; i < MAX_PROCESSES; i++) {
    if(active_queue[i] != NULL && active_queue[i]->ppid == process->pid) {
      return 1;
    }
  }
  return 0;
}

void switch_to(pcb *p1, pcb *p2){
  __asm__ volatile(
  "\
      pushq $1f;\
      pushq %%rsi;\
      pushq %%rdi;\
      pushq %%rbp;\
      pushq %%rax;\
      pushq %%rbx;\
      pushq %%rcx;\
      pushq %%rdx;\
      pushq %%r8;\
      pushq %%r9;\
      pushq %%r10;\
      pushq %%r11;\
      pushq %%r12;\
      pushq %%r13;\
      pushq %%r14;\
      pushq %%r15;\
    "
  :
  :
  );
  __asm__ volatile(
  "\
      movq %%rsp, %0\
    "
  :
  :"m" (p1->rsp)
  );
//  kprintf("Value in rsp p1 is %p\n", p1->rsp);
//  kprintf("Value in rsp p2 is %p\n", p2->rsp);
  __asm__ volatile(
  "\
      movq %0, %%rsp\
    "
  :
  :"m" (p2->rsp)
  );
  __asm__ volatile(
  "\
      popq %%r15;\
      popq %%r14;\
      popq %%r13;\
      popq %%r12;\
      popq %%r11;\
      popq %%r10;\
      popq %%r9;\
      popq %%r8;\
      popq %%rdx;\
      popq %%rcx;\
      popq %%rbx;\
      popq %%rax;\
      popq %%rbp;\
      popq %%rdi;\
      popq %%rsi;\
      retq;\
    "
  :
  :
  :"%rbp","%rdi","%rsi","%rax","%rbx","%rcx","%rdx","%r8","%r9","%r10","%r11","%r12","%r13","%r14","%r15"
  );
  __asm__ volatile("1:");

}

void p_set_process_name(pcb *process, char *name) {
  strcpy(process->name, name);
}

void p_switch_to_user_mode(pcb *process) {
  set_tss_rsp(&(process->kstack[127]));
  //These steps are referred from the James Molloy kernel development tutorials
  //Link: http://www.jamesmolloy.co.uk/tutorial_html/10.-User%20Mode.html
  __asm__ volatile("  \
		//cli; \
		movq $0x23, %%rax; \
		movq %%rax, %%ds; \
		movq %%rax, %%es; \
		movq %%rax, %%fs; \
		movq %%rax, %%gs; \
								 \
		//movq %0, %%rax; \
		pushq $0x23; \
		pushq %%rax; \
                 \
		pushfq; \
		popq %%rax ; \
		orq $0x200, %%rax ; \
		push %%rax ; \
                 \
		pushq $0x2B; \
		movq %1, %%rax; \
    pushq %%rax; \
		iretq; \
		"
    :
    : "r" (process->rsp), "r"(process->rip)
  );
}