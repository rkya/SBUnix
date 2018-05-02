#ifndef _STDLIB_H
#define _STDLIB_H

#include <sys/defs.h>
#include <environment.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

int main(int argc, char *argv[], char *envp[]);
void exit(int status);

void *malloc(size_t size);
void free(void *ptr);
void *realloc(void * , size_t );

int putenv(char * );
char * getenv(char * );

#endif
