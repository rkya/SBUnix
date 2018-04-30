#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <sys/defs.h>
#include <sys/registers.h>
#include <sys/kprintf.h>
//#include <sys/isr.h>

#define SYSCALL_FORMATTED_KPRINTF 1
#define SYSCALL_KMALLOC 2
#define SYSCALL_YIELD 3
#define SYSCALL_P_GET_PROCESS_BY_ID 4
#define SYSCALL_P_GET_CURRENT_PROCESS 5
#define SYSCALL_P_GET_PARENT_PROCESS 6
#define SYSCALL_P_REMOVE_PROCESS_BY_ID 7
#define SYSCALL_P_HAS_CHILD_PROCESSES 8
#define SYSCALL_T_GET_CHILDREN_INDEX 9
#define SYSCALL_T_TARFS_GET_INDEX 10
#define SYSCALL_T_READ_CHAR_FROM_SCREEN 11
#define SYSCALL_T_READ_LINE_FROM_SCREEN 12


uint64_t return_value;

void s_syscall_handler(registers *);

#endif
