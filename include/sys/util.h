#ifndef _UTIL_H
#define _UTIL_H

#include <sys/registers.h>

// I/O access:
static inline void u_save_state(registers *reg) {
  /*__asm__ __volatile__ (
      "movq %%rax, %0; \
       movq %%rbx, %1; \
       movq %%rcx, %2; \
       movq %%rdx, %3; \
       movq %%rdi, %4; \
       movq %%rsi, %5; \
       movq %%rbp, %6;\
       movq %%rsp, %7; "

    : "=r"(reg->rax),        *//* output *//*
      "=r"(reg->rbx),
      "=r"(reg->rcx),
      "=r"(reg->rdx),
      "=r"(reg->rdi),
      "=r"(reg->rsi),
      "=r"(reg->rbp),
      "=r"(reg->rsp)
    :                        *//* input *//*
    :                        *//* clobbered register *//*
  );*/



  //had to comment this because of system halt error
  /*__asm__ volatile(
  "\
      movq %%rsi, %14;\
      movq %%rdi, %13;\
      movq %%rbp, %12;\
      movq %%rax, %11;\
      movq %%rbx, %10;\
      movq %%rcx, %9;\
      movq %%rdx, %8;\
      movq %%r8,  %7;\
      movq %%r9,  %6;\
      movq %%r10, %5;\
      movq %%r11, %4;\
      movq %%r12, %3;\
      movq %%r13, %2;\
      movq %%r14, %1;\
      movq %%r15, %0;\
    "
  : "=r"(reg->r15),        *//* output *//*
    "=r"(reg->r14),
    "=r"(reg->r13),
    "=r"(reg->r12),
    "=r"(reg->r11),
    "=r"(reg->r10),
    "=r"(reg->r9),
    "=r"(reg->r8),
    "=r"(reg->rdx),
    "=r"(reg->rcx),
    "=r"(reg->rbx),
    "=r"(reg->rax),
    "=r"(reg->rbp),
    "=r"(reg->rdi),
    "=r"(reg->rsi)
  :
  );
  __asm__ volatile(
  "\
      movq %%rsp, %0\
    "
  :
  :"m" (reg->rsp)
  );*/
}

//add $0x10, %%rsp;

static inline void u_revive_state(registers *reg) {
  /*__asm__ __volatile__ (
      "movq %0, %%rax; \
       movq %1, %%rbx; \
       movq %2, %%rcx; \
       movq %3, %%rdx; \
       movq %4, %%rdi; \
       movq %5, %%rsi; \
       movq %6, %%rbp;\
       movq %7, %%rsp; \
       sti; \
       iretq; "

  :                       *//* output *//*
  : "r"(reg->rax),        *//* input *//*
    "r"(reg->rbx),
    "r"(reg->rcx),
    "r"(reg->rdx),
    "r"(reg->rdi),
    "r"(reg->rsi),
    "r"(reg->rbp),
    "r"(reg->rsp)
  : "%rax", "%rbx", "%rcx", "%rdx", "%rdi", "%rsi", "%rbp", "%rsp"         *//* clobbered register *//*
  );*/



  //had to remove this because of system halt error
  /*__asm__ volatile(
  "\
      movq %0, %%rsp\
    "
  :
  :"m" (reg->rsp)
  );
  __asm__ volatile(
  "\
      movq %0, %%r15;\
      movq %1, %%r14;\
      movq %2, %%r13;\
      movq %3, %%r12;\
      movq %4, %%r11;\
      movq %5, %%r10;\
      movq %6, %%r9;\
      movq %7, %%r8;\
      movq %8, %%rdx;\
      movq %9, %%rcx;\
      movq %10, %%rbx;\
      movq %11, %%rax;\
      movq %12, %%rbp;\
      movq %13, %%rdi;\
      movq %14, %%rsi;\
      pushq $1f;\
      retq;\
    "
  :
  : "r"(reg->r15),        *//* input *//*
    "r"(reg->r14),
    "r"(reg->r13),
    "r"(reg->r12),
    "r"(reg->r11),
    "r"(reg->r10),
    "r"(reg->r9),
    "r"(reg->r8),
    "r"(reg->rdx),
    "r"(reg->rcx),
    "r"(reg->rbx),
    "r"(reg->rax),
    "r"(reg->rbp),
    "r"(reg->rdi),
    "r"(reg->rsi)
  :"%rbp","%rdi","%rsi","%rax","%rbx","%rcx","%rdx","%r8","%r9","%r10","%r11","%r12","%r13","%r14","%r15"
  );
  __asm__ volatile("1:");*/
}

#endif
