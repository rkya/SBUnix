#ifndef _GDT_H
#define _GDT_H

#include <sys/defs.h>

void g_init_gdt();
void set_tss_rsp(void *rsp);

#endif