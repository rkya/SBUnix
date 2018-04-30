#ifndef _STDIO_H
#define _STDIO_H

#include <sys/defs.h>
#include <file.h>

static const int EOF = -1;
//static FILE *stderr = NULL;
//static FILE *stdin = NULL;
//static FILE *stdout = NULL;

int putchar(int c);
int puts(const char *s);
int printf(const char *format, ...);

FILE *fopen(char *, const char *);
int fprintf(FILE *, const char *, ...);
int feof(FILE *);
char getc(FILE *);
int fclose(FILE *);

char *gets(char *s);
char getchar();

#endif
