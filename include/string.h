#ifndef _STRING_H_
#define _STRING_H_

#include <stdlib.h>

int strcmp(const char *, const char *);
//int strsubcmp(char *, char *, int, int);
int strcpy(char *, char *);
int strsubcpy(char *, char *, int, int);
int strcat(char *, char *);
int strlen(const char *);
int strrev(char *);

char *strtok(char *, const char *);

#endif
