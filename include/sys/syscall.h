#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <sys/defs.h>
#include <sys/registers.h>
#include <sys/kprintf.h>
//#include <sys/isr.h>

#define SYSCALL_FORMATTED_KPRINTF 1

uint64_t return_value;

void s_syscall_handler(registers *);

#endif
