#ifndef _TARFS_H
#define _TARFS_H

//References: https://wiki.osdev.org/USTAR

#include <string.h>
#include <math.h>
#include <sys/defs.h>
#include <sys/process.h>
#include <sys/elf64.h>

#define MAX_VFS_SIZE 2048
#define TYPE_DIRECTORY '5'
#define TYPE_FILE '0'

extern char _binary_tarfs_start;
extern char _binary_tarfs_end;

struct posix_header_ustar {
  char name[100];
  char mode[8];
  char uid[8];
  char gid[8];
  char size[12];
  char mtime[12];
  char checksum[8];
  char typeflag[1];
  char linkname[100];
  char magic[6];
  char version[2];
  char uname[32];
  char gname[32];
  char devmajor[8];
  char devminor[8];
  char prefix[155];
  char pad[12];
};

// vfs data structure:
typedef struct virtual_file_system {
  char name[MAX_PATH_SIZE];
  int parent;
  uint64_t size;
  char type;
  uint64_t address;
  int index;
  int offset;
  char *contents;
  struct posix_header_ustar *tarfs_ptr;
} virtual_file_system;

virtual_file_system vfs[MAX_VFS_SIZE];
int vfs_size;


void t_init_tarfs();
void t_print_vfs();
int t_tarfs_get_index(const char *);
int t_get_children_index(const char *, int *, int);
ssize_t t_read(int , char *, uint64_t );

#endif
