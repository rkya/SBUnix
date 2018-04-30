#include <stdio.h>
#include <sys/tarfs.h>
#include <sys/process.h>


void f_init_file() {
  pcb *sbush_process = p_get_process_by_id(1);
  stdin = &(sbush_process->fd_array[0]);
  stdout = &(sbush_process->fd_array[1]);
  stderr = &(sbush_process->fd_array[2]);
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
  current_process->fd_array[current_process->fd_array_size].index = current_process->fd_array_size;
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
  if(buf == NULL) {
    return (ssize_t)0;
  }
  if(fd > 2) {
    return t_read(fd, (char *)buf, count);
  } else if(fd == 0) {
    pcb *current_process = p_get_current_process();

    char *buffer = buf;
    int i;
    int previous_position = current_process->fd_array[fd].position;
    int buffer_index = 0;
    int remaining_buffer_size = strlen(current_process->fd_array[fd].buffer) - current_process->fd_array[fd].position;
    if (remaining_buffer_size == 0) {
      gets(current_process->fd_array[fd].buffer);
      remaining_buffer_size = strlen(current_process->fd_array[fd].buffer);
    }
    for (i = current_process->fd_array[fd].position;
         i < count - 1 && i < remaining_buffer_size;
         i++, buffer_index++) {
      buffer[buffer_index] = current_process->fd_array[fd].buffer[i];
    }
    buffer[buffer_index] = '\0';

    //update the file_pointer
    current_process->fd_array[fd].position = i;

    if(i >= strlen(current_process->fd_array[fd].buffer)) {
      current_process->fd_array[fd].position = 0;
      current_process->fd_array[fd].buffer[0] = '\0';
    }

    //return the value min(size, remaining file)
    return (ssize_t)(i - previous_position);
  }
  return (ssize_t)0;
}

/***
 * Write to a file is not supported, hence this function just checks if the file descriptor is stdout. If this is true,
 * the content to be printed is passed to the printf() function.
 * @param fd file descriptor index of the file where buf is to be printed
 * @param buf data
 * @param count size of data
 * @return size of data written
 */
ssize_t write(int fd, const void *buf, size_t count) {
  if(fd == 1) {
    return printf(buf);
  }
  return (ssize_t)0;
}

FILE *fopen(char *file_name, const char *mode) {
  int open_ret_value = open(file_name, 1);
  if(open_ret_value == -1) {
    return NULL;
  }

  pcb *current_process = p_get_current_process();
  return &(current_process->fd_array[open_ret_value]);
}

/***
 * This function just checks if the file descriptor is stdout.
 * If this is true, the content to be printed is passed to the printf() function orelse write function is called.
 * @param file_ptr file descriptor of the file to be written
 * @param string to be written
 * @param ... additional optional parameters of the printf function
 * @return number of characters written
 */
int fprintf(FILE *file_ptr, const char *string, ...) {
  if(file_ptr == stdout || file_ptr == stderr) {
    return printf(string);
  }
  return (uint64_t) write(file_ptr->index, string, sizeof(string));
}

int feof(FILE *file_ptr) {
  int fd = file_ptr->index;
  if(fd > 2) {
    //get current process
    pcb *current_process = p_get_current_process();
    //check if the file descriptor is present
    if(fd >= current_process->fd_array_size || !current_process->fd_array[fd].live) {
      return 1;
    }
    //check if the file_pointer < size of file
    if(current_process->fd_array[fd].position >= vfs[current_process->fd_array[fd].file_pointer].size) {
      return 1;
    }

    if(current_process->fd_array[fd].position >= vfs[current_process->fd_array[fd].file_pointer].size) {
      return 1;
    }

    return 0;

  } else if(fd == 0) {
    pcb *current_process = p_get_current_process();

    if(current_process->fd_array[fd].buffer[0] == '\0' ||
       strlen(current_process->fd_array[fd].buffer) != 0) {
      return 0;
    }
    return 1;
  }
  return 1;
}

char getc(FILE *file_ptr) {
  if(file_ptr->index > 2) {
    char buffer[2];
    return read(file_ptr->index, buffer, (ssize_t)2);
  } else if(file_ptr->index == 0) {
    return getchar();
  }
  return (char)0;
}

int fclose(FILE *file_ptr) {
  return close(file_ptr->index);
}