#ifndef _FILE_H
#define _FILE_H

#include <sys/defs.h>
#include <sys/process.h>

FILE *stderr;
FILE *stdin;
FILE *stdout;

void f_init_file();

#endif
