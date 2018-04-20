#include <stdio.h>
#include <sys/tarfs.h>
#include <sys/process.h>

FILE *fopen(const char *file_name, const char *mode) {
  //TODO: Implement code to read file contents from tarfs...
  return NULL;
}

int fprintf(FILE *file_ptr, const char *string, ...) {
  //TODO: Write to file.
  return 0;
}

int feof(FILE *file_ptr) {
  //TODO: Find if we have reached the end of file.
  return 0;
}

char getc(FILE *file_ptr) {
  //TODO: Get the next character from the file.
  return '0';
}

int open(char *pathname, int flags) {
  //check if the file is present in vfs
  int vfs_index = t_tarfs_get_index(pathname);
  if(vfs_index == -1) {
    return -1;
  }

  //check if it is indeed a file and not a directory
  if(vfs[vfs_index].type == TYPE_DIRECTORY) {
    return -1;
  }

  //create a new file descriptor
  pcb *current_process = p_get_current_process();
  if(current_process->fd_array_size >= MAX_FILE_DESCRIPTORS) {
    return -1;
  }

  strcpy(current_process->fd_array[current_process->fd_array_size].name, pathname);
  current_process->fd_array[current_process->fd_array_size].position = 0;
  current_process->fd_array[current_process->fd_array_size].file_pointer = vfs_index;
  current_process->fd_array[current_process->fd_array_size].live = 1;
  current_process->fd_array_size++;

  //return this new file descriptor
  return current_process->fd_array_size - 1;
}

int close(int fd) {
  pcb *current_process = p_get_current_process();

  if(fd >= current_process->fd_array_size) {
    return 0;
  }

  current_process->fd_array[fd].live = 0;

  return 1;
}

ssize_t read(int fd, void *buf, size_t count) {
  return t_read(fd, (char *)buf, count);
}

ssize_t write(int fd, const void *buf, size_t count) {
  return (ssize_t)0;
}