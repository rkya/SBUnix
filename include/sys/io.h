#ifndef _IO_H
#define _IO_H

#include <sys/defs.h>

// I/O access:
static inline void outb(uint16_t port, uint8_t val)
{
  __asm__ __volatile__ (
      "outb %0, %1"
    :
    : "a"(val), "Nd"(port)
  );
}

static inline uint8_t inb(uint16_t port)
{
  uint8_t ret;

  __asm__ __volatile__ (
      "inb %1, %0"
    : "=a"(ret)
    : "Nd"(port)
  );

  return ret;
}

#endif
