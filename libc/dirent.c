#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/tarfs.h>

static DIR cache_directory;

DIR *opendir(char *name) {
  strcpy(cache_directory.path, name);
  cache_directory.current_vfs_index = 0;
  cache_directory.vfs_indexes_size = t_get_children_index(name, cache_directory.vfs_indexes, MAX_VFS_INDEXES);
  return &cache_directory;

  /*DIR *dirp = (DIR *) malloc(sizeof(DIR));

  if(dirp == NULL) {
    return NULL;
  } else {
    strcpy(dirp->path, name);
    dirp->current_vfs_index = 0;
    dirp->vfs_indexes_size = t_get_children_index(name, dirp->vfs_indexes, MAX_VFS_INDEXES);
    return dirp;
  }*/
}

struct dirent *readdir(DIR *dirp) {
  if(dirp->current_vfs_index >= dirp->vfs_indexes_size) {
    return NULL;
  }
  strcpy(dirp->current_dirent.d_name, vfs[dirp->vfs_indexes[dirp->current_vfs_index]].name);
  dirp->current_dirent.d_size = vfs[dirp->current_vfs_index].size;
  dirp->current_dirent.d_type = vfs[dirp->current_vfs_index].type;
  dirp->current_dirent.d_offset = vfs[dirp->current_vfs_index].offset;
  dirp->current_vfs_index++;
  return &(dirp->current_dirent);
}

int closedir(DIR *dirp) {
  /*if(dirp != NULL) {
    free(dirp);
  }*/
  return 1;
}