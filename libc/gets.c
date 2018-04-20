#include <stdio.h>
#include <string.h>
#include <sys/terminal.h>

char *gets(char *s) {
  char *arr = t_read_line_from_screen();
  strcpy(s, arr);
  return arr;
}