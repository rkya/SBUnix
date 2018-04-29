#include <stdio.h>
#include <string.h>
#include <sys/terminal.h>
#include <sys/syscall.h>

//extern int return_value;

int printf(const char *format, ...) {
  /*char *arr = t_read_line_from_screen();
  char ds[3];
  strcpy(ds, arr);*/
  int val;
  __asm__ __volatile__ (
  "int $0x80;"
  : "=a" (val)
  : "0"(SYSCALL_KPRINTF), "D"(format)
  : "cc", "rcx", "r11", "memory"
  );

  return_value = 22;
  val = return_value;
  return val;
}
