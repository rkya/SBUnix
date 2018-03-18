/*
#ifndef _GDT_H
#define _GDT_H

void g_init_gdt();
void set_tss_rsp(void *rsp);

#endif
*/

#ifndef _GDT_H
#define _GDT_H

#include <sys/defs.h>

void init_gdt();
void set_tss_rsp(void *rsp);
//void write_tss(int64_t , uint32_t , uint64_t );

#endif
