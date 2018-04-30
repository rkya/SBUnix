#include <sys/syscall.h>

void s_syscall_handler(registers *regs)
{
  //kprintf("Inside syscall handler\n");
  uint64_t syscall_number = regs->rax;

  switch (syscall_number) {
    case SYSCALL_FORMATTED_KPRINTF:
      kprintf_formatted((const char *) regs->rdi);
      //kprintf("Proper syscall is getting called.\n");
      uint64_t val = 7;
      __asm__ volatile("movq %0, %%rax;"::"r"(val));
      return_value = 8;
      break;
  }
}