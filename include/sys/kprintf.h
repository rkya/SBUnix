#ifndef _KPRINTF_H
#define _KPRINTF_H

#include <sys/terminal.h>

void kprintf(const char *fmt, ...);
void kprintf_formatted(const char *fmt);

#endif
