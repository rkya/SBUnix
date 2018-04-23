#include <sys/syscall.h>

void s_syscall_handler(registers *regs)
{
  kprintf("Inside syscall handler\n");
  uint64_t syscall_number = regs->rax;

  switch (syscall_number) {
    case SYSCALL_KPRINTF:
      kprintf("Proper syscall is getting called.\n");
      regs->rax = 4;
      break;
  }
}