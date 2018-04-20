#ifndef _DIRENT_H
#define _DIRENT_H

#include <sys/defs.h>

#define NAME_MAX 255
#define MAX_VFS_INDEXES 128

//Contents of dirent structure: https://stackoverflow.com/questions/12991334/members-of-dirent-structure
struct dirent {
  char d_name[NAME_MAX+1];
  char d_type;
  int d_offset;
  uint16_t d_size;
  //can add inode number if needed
};

typedef struct DIR {
  char path[128];
  //char *path;
  int vfs_indexes[MAX_VFS_INDEXES];
  int vfs_indexes_size;
  int current_vfs_index;
  struct dirent current_dirent;
} DIR;

DIR *opendir(char *name);
struct dirent *readdir(DIR *dirp);
int closedir(DIR *dirp);

#endif
