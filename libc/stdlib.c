#include <stdlib.h>
#include <sys/paging.h>
#include <sys/mm.h>
#include <sys/process.h>
#include <sys/tarfs.h>

void *malloc(size_t size) {
  //TODO: instead of calling kmalloc, implement a code which will book user level pages.
  void *new_pages = (void *) kmalloc(size);
  return new_pages;
}

void free(void *ptr) {
  //TODO: Free the allocated page and add it to the free list.
}

void *realloc(void *ptr , size_t size) {
  void *new_pages = (void *) kmalloc(size);
  memcpy(new_pages, ptr, size);
  return new_pages;
}

void exit(int status) {
  pcb *current_process = p_get_current_process();
  pcb *parent_process = p_get_parent_process(current_process);

  if(current_process == NULL) {
    return;
  }
  current_process->exit_status = status;
  if(parent_process != NULL && (parent_process->wait_return_pid == -1 || parent_process->wait_return_pid == current_process->pid)) {
    parent_process->wait_return_pid = current_process->pid;
    parent_process->state = RUNNING;
  }

  current_process->state = ZOMBIE;
  yield();
}