#include <stdio.h>
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

int open(const char *pathname, int flags) {
  //check if the file is present in vfs
  //check if it is indeed a file and not a directory
  //create a new file descriptor
  //return this new file descriptor
  return 0;
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
  return (ssize_t)0;
}

ssize_t write(int fd, const void *buf, size_t count) {
  return (ssize_t)0;
}