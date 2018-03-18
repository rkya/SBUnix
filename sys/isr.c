#include <sys/isr.h>
#include <sys/kprintf.h>
//#include <sys/syscall.h>

/* implementation details for isr are referred from http://www.osdever.net/bkerndev/Docs/isrs.htm */

char *exception_messages[] = {
  "Divide by Zero Exception",
  "Debug Exception",
  "NMI Exception",
  "Breakpoint Exception",
  "Into Detected Overflow Exception",
  "Out of bounds Exception",
  "Invalid Opcode Exception",
  "No Coprocessor Exception",
  "Double Fault Exception",
  "Coprocessor Segment Overrun Exception",
  "Bad TSS Exception",
  "Segment Not Present Exception",
  "Stack Fault Exception",
  "General Protection Fault Exception",
  "Page Fault Exception",
  "Unknown Interrupt Exception",
  "Coprocessor Fault Exception",
  "Alignment Check Exception (486+)",
  "Machine Check Exception (Pentium/586+)",
  "Reserved Exceptions",
  "Reserved Exceptions",
  "Reserved Exceptions",
  "Reserved Exceptions",
  "Reserved Exceptions",
  "Reserved Exceptions",
  "Reserved Exceptions",
  "Reserved Exceptions",
  "Reserved Exceptions",
  "Reserved Exceptions",
  "Reserved Exceptions",
  "Reserved Exceptions",
  "Reserved Exceptions"
};

void fault_handler(registers r)
{
  kprintf("in fault handler\n");
  kprintf("r.int_no = %d\n", r.int_no);
  if (r.int_no < 32) {
    kprintf("%s", exception_messages[r.int_no]);
    kprintf(" Exception. System Halted!\n");
  }
  outb(0x20, 0x20);
}

void irq_handler(registers r)
{
  if(r.int_no == 32) {
    outb(0x20, 0x20);
    return;
  }
  if (r.int_no >= 40 && r.int_no != 128) {
    kprintf("r.int_no = %d. Exception. System Halted!\n", r.int_no);
    // send reset signal to slave
    outb(0xA0, 0x20);
  }
  if(r.int_no == 33) {
    keyboard_interrupt();
  }
  if(r.int_no == 128) {
    //syscall_handler(&r);
  }

  // send reset signal to master
  outb(0x20, 0x20);
}
