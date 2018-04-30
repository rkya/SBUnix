#include <sys/syscall.h>
#include <sys/tarfs.h>
#include <sys/process.h>

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
      return_value2 = return_value;
      break;
    case SYSCALL_P_GET_PROCESS_BY_ID:
      return_value = (uint64_t)p_get_process_by_id((int) regs->rdi);
      return_value2 = return_value;
      break;
    case SYSCALL_T_TARFS_GET_INDEX:
      return_value = t_tarfs_get_index((const char *) regs->rdi);
      return_value2 = return_value;
      break;
    case SYSCALL_P_GET_CURRENT_PROCESS:
      return_value = (uint64_t ) p_get_current_process();
      return_value2 = return_value;
//      kprintf("return val = %p\n", return_value2);
    default:
      return_value = -1;
      break;
  }
}