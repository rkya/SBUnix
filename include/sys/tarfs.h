#ifndef _TARFS_H
#define _TARFS_H

#include <string.h>
#include <math.h>
#include <sys/defs.h>

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
} virtual_file_system;

virtual_file_system vfs[MAX_VFS_SIZE];
int vfs_size;


void t_init_tarfs();
void t_print_vfs();
/*uint64_t tar_lookup(char *);
uint64_t tar_ls(char *);
int tar_get_index(char *);
void get_file_content(char *);
int test_offset(char *);
int get_file_offset(char *);
//Elf64_Ehdr *get_elf(char *);
struct posix_header_ustar *get_tarfs(char *);
struct posix_header_ustar *traverse_tarfs(char *);
struct file *open_tarfs(struct posix_header_ustar *);*/

#endif
