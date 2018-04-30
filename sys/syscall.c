#include <sys/syscall.h>
#include <sys/tarfs.h>

void s_syscall_handler(registers *regs)
{
  //kprintf("Inside syscall handler\n");
  uint64_t syscall_number = regs->rax;

  switch (syscall_number) {
    case SYSCALL_FORMATTED_KPRINTF:
      kprintf_formatted((const char *) regs->rdi);
      //kprintf("Proper syscall is getting called.\n");
//      uint64_t val = 7;
//      __asm__ volatile("movq %0, %%rax;"::"r"(val));
//      return_value = 8;
      break;
    case SYSCALL_T_GET_CHILDREN_INDEX:
      return_value = t_get_children_index((const char *)regs->rdi, (int *)regs->rsi, (int)regs->rdx);
//      kprintf("Syscall value to be returned = %d.\n", return_value);
      break;
  }
}