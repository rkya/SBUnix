#ifndef _IDT_H
#define _IDT_H

#include <sys/defs.h>
#include <sys/io.h>

void i_init_idt();

struct idt_t {
  uint16_t low_offset; // offset bits 0..15
  uint16_t selector; // a code segment selector in GDT or LDT
  uint8_t ist;       // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
  uint8_t type; // type and attributes
  uint16_t mid_offset; // offset bits 16..31
  uint32_t high_offset; // offset bits 32..63
  uint32_t zero;     // reserved
}__attribute__((packed));

struct idtr_t {
  uint16_t limit;
  uint64_t base;
}__attribute__((packed));

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void isr32();

// IRQs:
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();
extern void irq128();


#endif


/*
#ifndef _IDT_H
#define _IDT_H

#include <sys/defs.h>
#include <sys/io.h>
//#include <sys/syscall.h>

void i_init_idt();
void idt_install();
void idt_set_gate(int , uint64_t , uint16_t , uint16_t , int16_t , uint16_t , uint16_t );

struct idt_t {
  uint16_t low_offset; // offset bits 0..15
  uint16_t selector; // a code segment selector in GDT or LDT
  uint8_t ist;       // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
  uint8_t type; // type and attributes
  uint16_t mid_offset; // offset bits 16..31
  uint32_t high_offset; // offset bits 32..63
  uint32_t zero;     // reserved
}__attribute__((packed));

*/
/*struct idt_t {
  uint16_t low_offset;
  uint16_t selector;
  uint16_t ist :3;
  uint16_t reserved0 :1;
  uint16_t reserved1 :1;
  uint16_t reserved2 :3;
  uint16_t type:4;
  uint16_t reserved3 :1;
  uint16_t dpl :2;
  uint16_t p :1;
  uint16_t mid_offset;
  uint32_t high_offset;
  uint16_t reserved;
}__attribute__((packed));
*//*



struct idtr_t {
  uint16_t limit;
  uint64_t base;
}__attribute__((packed));



extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void isr32();


// IRQs:

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();
extern void irq128();


#endif

*/
