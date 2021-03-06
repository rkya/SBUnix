#include <stdlib.h>
#include <sys/paging.h>
#include <sys/mm.h>
#include <sys/process.h>
#include <sys/tarfs.h>
#include <sys/syscall.h>

void *malloc(size_t size) {
//  void *new_pages = (void *) kmalloc(size);
  int temp_val;
  __asm__ __volatile__ (
  "int $0x80;"
  : "=a" (temp_val)
  : "0"(SYSCALL_KMALLOC), "D"(size)
  : "cc", "rcx", "r11", "memory"
  );
  void *new_pages = (void *)return_value2;

  return new_pages;
}

void free(void *ptr) {

}

void *realloc(void *ptr , size_t size) {
  void *new_pages = malloc(size);
  memcpy(new_pages, ptr, size);
  free(ptr);
  return new_pages;
}

void exit(int status) {
//  pcb *current_process = p_get_current_process();
  int val = 0;
  __asm__ __volatile__ (
  "int $0x80;"
  : "=a" (val)
  : "0"(SYSCALL_P_GET_CURRENT_PROCESS)
  : "cc", "rcx", "r11", "memory"
  );
  pcb *current_process = (pcb *)return_value;

  if(current_process == NULL) {
    return;
  }

//  pcb *parent_process = p_get_parent_process(current_process);
  int temp_val;
  __asm__ __volatile__ (
  "int $0x80;"
  : "=a" (temp_val)
  : "0"(SYSCALL_P_GET_PARENT_PROCESS), "D"(current_process)
  : "cc", "rcx", "r11", "memory"
  );
  pcb *parent_process = (pcb *)return_value2;


  current_process->exit_status = status;
  if(parent_process != NULL && (parent_process->wait_return_pid == -1 || parent_process->wait_return_pid == current_process->pid)) {
    parent_process->wait_return_pid = current_process->pid;
    parent_process->state = RUNNING;
  }

  current_process->state = ZOMBIE;
  yield();
}