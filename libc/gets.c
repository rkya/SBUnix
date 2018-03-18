#include <stdio.h>
#include <sys/terminal.h>

char *gets(char *s) {
  return t_read_line_from_screen();
}