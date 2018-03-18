#ifndef _REGISTERS_H
#define _REGISTERS_H

#include <sys/defs.h>

typedef struct {
  uint64_t r8, r9, r10, r11, r12, r13, r14, r15;
  uint64_t ds;
  uint64_t rdi, rsi, rbp, rsp, rbx, rdx, rcx, rax;
  uint64_t int_no, err_code;
  uint64_t rip, cs, rflags, useresp, ss;  // Pushed by the processor automatically.
} registers;

#endif