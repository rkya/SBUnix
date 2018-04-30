#ifndef _KPRINTF_H
#define _KPRINTF_H

//#include <stdarg.h>
//#include <stdlib.h>
#include <sys/terminal.h>

void kprintf(const char *fmt, ...);
void kprintf_formatted(const char *fmt);
//void initScreen();
//void scroll();
//void setNewVideoCardAddresses();

#endif
