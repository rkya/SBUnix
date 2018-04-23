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
  //TODO: implement
  return (pid_t)0;
}

int execvpe(const char *file, char *const argv[], char *const envp[]) {
  //TODO: implement
  return 0;
}

int execvp(const char *file, char *const argv[]) {
  //TODO: implement
  return 0;
}

pid_t wait(int *status) {
  //TODO: implement
  return (pid_t)0;
}

int waitpid(int pid, int *status) {
  //TODO: implement
  return 0;
}

unsigned int sleep(unsigned int seconds) {
  pcb *current_process = p_get_current_process();
  current_process->remaining_sleep_time = seconds;
  current_process->state = SLEEPING;

  /*while (current_process->remaining_sleep_time > 0) {
    yield();
  }*/
  yield();

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