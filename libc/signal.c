#include <signal.h>
#include <stdio.h>

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
  int ret_val = p_remove_process_by_id(pid);

  //if process to be killed is current process, yield.
  if(ret_val == 1) {
    pcb *current_process = p_get_current_process();
    if(pid == current_process->pid) {
      yield();
    }
    return 0;
  }

  return -1;
}

void shutdownOS() {
  //keep only the current process alive
  pcb *current_process = p_get_current_process();

  //remove all processes from active queue
  for(int i = 0; i < MAX_PROCESSES; i++) {
    if(i != current_process->pid) {
      p_remove_process_by_id((pid_t) i);
    }
  }

  printf("\nIt's now safe to turn off your computer.");

  __asm__ volatile ("cli");

  //halt the cpu
  while(1) {
    __asm__ volatile ("hlt");
  }
}