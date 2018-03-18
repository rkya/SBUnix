#include <stdio.h>
#include <sys/terminal.h>

char getchar() {
  return t_read_char_from_screen();
}