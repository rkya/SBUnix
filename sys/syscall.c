#include <sys/syscall.h>
#include <sys/tarfs.h>
#include <sys/process.h>
#include <sys/terminal.h>

//References: https://wiki.osdev.org/System_Calls
//http://www.jamesmolloy.co.uk/tutorial_html/10.-User%20Mode.html

void s_syscall_handler(registers *regs)
{
  uint64_t syscall_number = regs->rax;

  switch (syscall_number) {
    case SYSCALL_FORMATTED_KPRINTF:
      kprintf_formatted((const char *) regs->rdi);
      break;
    case SYSCALL_T_GET_CHILDREN_INDEX:
      return_value = t_get_children_index((const char *)regs->rdi, (int *)regs->rsi, (int)regs->rdx);
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
      break;
    case SYSCALL_T_READ_LINE_FROM_SCREEN:
      return_value = (uint64_t) t_read_line_from_screen();
      return_value2 = return_value;
      break;
    case SYSCALL_P_REMOVE_PROCESS_BY_ID:
      return_value = p_remove_process_by_id((pid_t) regs->rdi);
      return_value2 = return_value;
      break;
    case SYSCALL_KMALLOC:
      return_value = (uint64_t) kmalloc((size_t) regs->rdi);
      return_value2 = return_value;
      break;
    case SYSCALL_P_GET_PARENT_PROCESS:
      return_value = (uint64_t) p_get_parent_process((pcb *) regs->rdi);
      return_value2 = return_value;
      break;
    default:
      return_value = -1;
      break;
  }
}