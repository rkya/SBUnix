#ifndef _STDLIB_H
#define _STDLIB_H

#include <sys/defs.h>
#include <environment.h>
#include <main_function.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

void exit(int status);

void *malloc(size_t size);
void free(void *ptr);
void *realloc(void * , size_t );

int putenv(char * );
char * getenv(char * );

#endif
