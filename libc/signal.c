#include <signal.h>
#include <stdio.h>
#include <sys/syscall.h>

int kill(pid_t pid, int sig) {
  if(sig != SIGKILL || pid < 0) {
    return -1;
  }

  if(pid == 1) {
    printf("Do you really want to shut down the OS? [y/Y]\n");
    char answer = getchar();
    if(answer == 'y' || answer == 'Y') {
      shutdownOS();
    }
  }

  //remove the process from active_queue
//  int ret_val = p_remove_process_by_id(pid);
  int val;
  __asm__ __volatile__ (
  "int $0x80;"
  : "=a" (val)
  : "0"(SYSCALL_P_REMOVE_PROCESS_BY_ID), "D"(pid)
  : "cc", "rcx", "r11", "memory"
  );
  int ret_val = return_value;

  //if process to be killed is current process, yield.
//  pcb *current_process = p_get_current_process();
  if(ret_val == 1) {
    int temp_val = 0;
    __asm__ __volatile__ (
    "int $0x80;"
    : "=a" (temp_val)
    : "0"(SYSCALL_P_GET_CURRENT_PROCESS)
    : "cc", "rcx", "r11", "memory"
    );
    pcb *current_process = (pcb *)return_value2;

    if(pid == current_process->pid) {
      yield();
    }
    return 0;
  }

  return -1;
}

void shutdownOS() {
  //keep only the current process alive
//  pcb *current_process = p_get_current_process();
  int temp_val = 0;
  __asm__ __volatile__ (
  "int $0x80;"
  : "=a" (temp_val)
  : "0"(SYSCALL_P_GET_CURRENT_PROCESS)
  : "cc", "rcx", "r11", "memory"
  );
  pcb *current_process = (pcb *)return_value2;

  //remove all processes from active queue
  for(int i = 0; i < MAX_PROCESSES; i++) {
    if(i != current_process->pid) {
//      p_remove_process_by_id((pid_t) i);
      int val;
      __asm__ __volatile__ (
      "int $0x80;"
      : "=a" (val)
      : "0"(SYSCALL_P_REMOVE_PROCESS_BY_ID), "D"(i)
      : "cc", "rcx", "r11", "memory"
      );
    }
  }

  printf("\nIt's now safe to turn off your computer.");

  __asm__ volatile ("cli");

  //halt the cpu
  while(1) {
    __asm__ volatile ("hlt");
  }
}