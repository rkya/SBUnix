#include <stdio.h>
#include <sys/terminal.h>

char getchar() {
  char buffer[2048];
  gets(buffer);
  return buffer[0];
}