#include <sys/idt.h>
#include <sys/kprintf.h>

//References: http://www.jamesmolloy.co.uk/tutorial_html/4.-The%20GDT%20and%20IDT.html

static struct idt_t idt[256];
static struct idtr_t idtr;
void _x86_64_asm_lidt(struct idtr_t*);
void idt_set_gate(int , uint64_t , uint16_t , uint16_t , int16_t , uint16_t , uint16_t );

void idt_set_gate(int index, uint64_t offset, uint16_t selector, uint16_t ist, int16_t type, uint16_t dpl, uint16_t p) {
  idt[index].low_offset = offset & 0xffff;
  idt[index].selector = selector;
  idt[index].ist = ist;
  idt[index].type = type;
  idt[index].mid_offset = (offset >> 16) & 0xffff;
  idt[index].high_offset = (offset >> 32) & 0xffffffff;
  idt[index].zero = 0;
}

/***
 * Initialize IDT.
 */

/*PIC code referred from: http://www.jamesmolloy.co.uk/tutorial_html/5.-IRQs%20and%20the%20PIT.html */

void i_init_idt() {
  idtr.limit = (sizeof(struct idt_t) * 256);
  idtr.base = (uint64_t)&idt;

  //memset to zero:
  uint8_t *temp = (uint8_t *)idtr.base;
  for(uint32_t length = sizeof(struct idt_t) * 256; length > 0; length--) {
    *temp++ = (uint8_t)0;
  }

  //idt_set_gate:

  idt_set_gate(0, (uint64_t)isr0, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(1, (uint64_t)isr1, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(2, (uint64_t)isr2, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(3, (uint64_t)isr3, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(4, (uint64_t)isr4, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(5, (uint64_t)isr5, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(6, (uint64_t)isr6, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(7, (uint64_t)isr7, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(8, (uint64_t)isr8, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(9, (uint64_t)isr9, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(10, (uint64_t)isr10, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(11, (uint64_t)isr11, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(12, (uint64_t)isr12, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(13, (uint64_t)isr13, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(14, (uint64_t)isr14, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(15, (uint64_t)isr15, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(16, (uint64_t)isr16, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(17, (uint64_t)isr17, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(18, (uint64_t)isr18, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(19, (uint64_t)isr19, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(20, (uint64_t)isr20, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(21, (uint64_t)isr21, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(22, (uint64_t)isr22, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(23, (uint64_t)isr23, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(24, (uint64_t)isr24, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(25, (uint64_t)isr25, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(26, (uint64_t)isr26, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(27, (uint64_t)isr27, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(28, (uint64_t)isr28, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(29, (uint64_t)isr29, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(30, (uint64_t)isr30, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(31, (uint64_t)isr31, 0x08, 0, 0x8e, 0, 1);

  __asm__ __volatile__ ("sti");

/*load idt table*/

  _x86_64_asm_lidt(&idtr);

  // Remap the IRQ table:
  outb(0x20, 0x11);
  outb(0xA0, 0x11);
  outb(0x21, 0x20);
  outb(0xA1, 0x28);
  outb(0x21, 0x04);
  outb(0xA1, 0x02);
  outb(0x21, 0x01);
  outb(0xA1, 0x01);
  outb(0x21, 0x0);
  outb(0xA1, 0x0);



/*
    Master - command: 0x20, data: 0x21
    Slave - command: 0xA0, data: 0xA1
  */

  idt_set_gate(32, (uint64_t)irq0, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(33, (uint64_t)irq1, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(34, (uint64_t)irq2, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(35, (uint64_t)irq3, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(36, (uint64_t)irq4, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(37, (uint64_t)irq5, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(38, (uint64_t)irq6, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(39, (uint64_t)irq7, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(40, (uint64_t)irq8, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(41, (uint64_t)irq9, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(42, (uint64_t)irq10, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(43, (uint64_t)irq11, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(44, (uint64_t)irq12, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(45, (uint64_t)irq13, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(46, (uint64_t)irq14, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(47, (uint64_t)irq15, 0x08, 0, 0x8e, 0, 1);
  idt_set_gate(128, (uint64_t)irq128, 0x08, 0, 0xee, 0, 1);
}
