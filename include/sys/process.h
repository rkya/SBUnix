#ifndef __TASK_H
#define __TASK_H

#include <sys/defs.h>
#include <sys/mm.h>
#include <sys/gdt.h>
#include <sys/paging.h>
#include <string.h>

#define MAX_PROCESSES 512

typedef struct pcb {
  uint64_t kstack[128];
  uint64_t stack[512]; //This stack should be accessible by the user
  char name[128];
  char current_directory[MAX_PATH_SIZE];
  uint64_t pid;
  uint64_t ppid;
  uint64_t rsp;
  enum {RUNNING, SLEEPING, ZOMBIE} state;
  int exit_status;
  mm_struct *mm_struct_ptr;
  uint64_t pml4e;
} pcb;

pcb *active_queue[MAX_PROCESSES];
uint64_t active_queue_size;
uint64_t current_process;
uint64_t pid_allocator_index;

void schedule();
void yield();
pcb *p_get_new_process(char *);
void p_set_process_name(pcb *, char *);
void p_init_process();
pcb *p_get_current_process();
int p_remove_process(pcb *);
void p_switch_to_user_mode();

#endif
