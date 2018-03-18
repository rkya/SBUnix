#ifndef _DEFS_H
#define _DEFS_H

#define NULL ((void*)0)
#define KERNBASE 0xffffffff80000000
#define MAX_PATH_SIZE 128

typedef unsigned long  uint64_t;
typedef          long   int64_t;
typedef unsigned int   uint32_t;
typedef          int    int32_t;
typedef unsigned short uint16_t;
typedef          short  int16_t;
typedef unsigned char   uint8_t;
typedef          char    int8_t;

typedef uint64_t size_t;
typedef int64_t ssize_t;

typedef uint64_t off_t;

typedef uint64_t pid_t;

typedef uint64_t pml4;
typedef uint64_t pdp;
typedef uint64_t pd;
typedef uint64_t pt;

typedef uint64_t FILE;

#endif
