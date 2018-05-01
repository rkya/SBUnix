#include <unistd.h>
#include <sys/process.h>
#include <sys/tarfs.h>

extern int vfs_size;
extern virtual_file_system vfs[MAX_VFS_SIZE];


int chdir(const char *path) {
  for(int i = 0; i < vfs_size; i++) {
    if(!strcmp(path, vfs[i].name)) {
      pcb *current_process = p_get_current_process();
      strcpy(current_process->current_directory, vfs[i].name);
      return 1;
    }
  }
  return 0;
}

char *getcwd(char *buf, size_t size) {
  pcb *current_process = p_get_current_process();
  int min = strlen(current_process->current_directory);
  if(min > size - 1) {
    min = size - 1;
  }
  memcpy(buf, current_process->current_directory, min);
  buf[min] = '\0';
  return current_process->current_directory;
}

pid_t fork() {
  return (pid_t)0;
}

int execvpe(const char *file, char *const argv[], char *const envp[]) {
  return 0;
}

int execvp(const char *file, char *const argv[]) {
  return 0;
}

pid_t wait(int *status) {
  return (pid_t) waitpid(-1, status);
}

int waitpid(int pid, int *status) {
  pcb *current_process = p_get_current_process();
  current_process->wait_return_pid = (pid_t) pid;

  if(p_has_child_processes(current_process)) {
    current_process->state = WAITING;
    yield();
  }

  if(current_process->wait_return_pid != -1 &&
    active_queue[current_process->wait_return_pid] != NULL &&
    status != NULL) {
    *status = active_queue[current_process->wait_return_pid]->exit_status;
  }

  return (uint64_t) current_process->wait_return_pid;
}

unsigned int sleep(unsigned int seconds) {
  /*pcb *current_process = p_get_current_process();
  current_process->remaining_sleep_time = seconds;
  current_process->state = SLEEPING;*/

  while (seconds > 10) {
    for (uint64_t i = 0; i < 2395882353/*2500000000*/; i++);
    seconds -= 10;
  }
  for (uint64_t i = 0; i < /*235588235*/350000000 * seconds; i++);
  /*while (current_process->remaining_sleep_time > 0) {
    yield();
  }*/
//  yield();

  return 0;
}

pid_t getpid(void) {
  pcb *current_process = p_get_current_process();
  return (pid_t)current_process->pid;
}

pid_t getppid(void) {
  pcb *current_process = p_get_current_process();
  return (pid_t)current_process->ppid;
}