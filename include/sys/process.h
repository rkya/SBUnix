#ifndef __TASK_H
#define __TASK_H

#include <sys/defs.h>
#include <sys/mm.h>
#include <sys/gdt.h>
#include <sys/paging.h>
#include <sys/elf64.h>
#include <string.h>

#define MAX_PROCESSES 512
#define MAX_FILE_DESCRIPTORS 64

//Difference between file descriptor and file pointer: https://forum.osdev.org/viewtopic.php?f=1&t=9031
//Information about file descriptor: https://stackoverflow.com/questions/12696315/how-much-information-is-actually-stored-in-a-file-descriptor
//More on file descriptors: https://www.bottomupcs.com/file_descriptors.xhtml

typedef struct file_descriptor {
  char name[32];
  char buffer[256];
  int index;
  int file_pointer; //this is the index to vfs from tarfs.h
  char mode[3]; //can be rwx - read, write and execute
  int position; //seek position in the file - can change the data type later based on implementation
  int live; //1 if the fd is live, 0 otherwise
} FILE; //this is FILE *fd

typedef struct pcb {
  uint64_t kstack[128];
  uint64_t stack[512]; //This stack should be accessible by the user
  char name[128];
  char current_directory[MAX_PATH_SIZE];
  pid_t pid;
  pid_t ppid;
  uint64_t rsp;
  uint64_t rip;
  enum {RUNNING, SLEEPING, WAITING, ZOMBIE, TERMINATED} state;
  pid_t wait_return_pid;
  int exit_status;
  mm_struct *mm_struct_ptr;
  uint64_t pml4e;
  FILE fd_array[MAX_FILE_DESCRIPTORS]; //also can be typecasted to FILE **fd_array
  int fd_array_size;
  unsigned int remaining_sleep_time;
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
pcb *p_get_process_by_id(int );
pcb *p_get_parent_process(pcb *);
int p_remove_process(pcb *);
int p_remove_process_by_id(pid_t process_id);
void p_switch_to_user_mode(pcb *);
void p_print_all_active_processes();
int p_has_child_processes(pcb *);

#endif
