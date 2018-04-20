#include <sys/tarfs.h>
#include <sys/kprintf.h>

void vfs_add(struct posix_header_ustar *tarfs_ptr, int offset, char *contents);
int get_parent_index(char *file_name, int slash_count);
void vfs_insert(int index, char *name, int parent_index, uint64_t size, char file_type,
                uint64_t address, int offset, char *contents);


/***
 * Inserts the vfs entry at the specified index of the vfs array
 * @param index where entry is to be made
 * @param name of the file / directory
 * @param parent_index of the file / directory
 * @param size of the file
 * @param file_type whether it is a file or a directory
 * @param address of the file / directory
 * @param offset of the file / directory
 */
void vfs_insert(int index, char *name, int parent_index, uint64_t size, char file_type,
                uint64_t address, int offset, char *contents) {
  strcpy(vfs[index].name, name);
  vfs[index].parent = parent_index;
  vfs[index].size = size;
  vfs[index].type = file_type;
  vfs[index].address = address;
  vfs[index].index = index;
  vfs[index].offset = offset;
  vfs[index].contents = contents;
}

/***
 * Returns the index of the parent directory from the VFS.
 * @param file_name whose parent directory is to be returned
 * @param slash_count depends on whether it is a file(1) or a directory(2)
 * @return index of the the parent directory in the VFS
 */
int get_parent_index(char *file_name, int slash_count) {
  int length = strlen(file_name);
  int i;
  for(i = length - 1; i >= 0 && slash_count; i--) {
    if(file_name[i] == '/') {
      slash_count--;
    }
  }
  if(i <= -1) {
    return -1;
  }

  char fname[128];
  //here, i points to the last character of the file_name without the '/'
  //we have to include '/' while entering it in vfs, hence we use i + 2;
  strsubcpy(fname, file_name, 0, i + 2);
  fname[i + 2] = '\0';

  for(i = 0; i < vfs_size; i++) {
    if(!strcmp(fname, vfs[i].name)) {
      return i;
    }
  }
  return -2;
}

/***
 * Add a single entry in the VFS from the tarfs.
 * @param tarfs_ptr to the contents in tarfs
 * @param offset from the current file
 */
void vfs_add(struct posix_header_ustar *tarfs_ptr, int offset, char *contents) {
  char file_type = tarfs_ptr->typeflag[0];

  if(vfs_size >= MAX_VFS_SIZE || (file_type != TYPE_DIRECTORY && file_type != TYPE_FILE)) {
    return;
  }

  char file_name[128];
  strcpy(file_name, "/rootfs/");
  strcat(file_name, tarfs_ptr->name);
  uint64_t file_size = octal_to_binary((unsigned char *)tarfs_ptr->size, 11);
  uint64_t address = (uint64_t)tarfs_ptr + file_size;

  /*kprintf(" name=%s ", file_name);
  //kprintf("linkname=%s ", tarfs_ptr->linkname);
  kprintf("uname=%s ", tarfs_ptr->uname);
  //kprintf("prefix=%s  ", tarfs_ptr->prefix);
  kprintf("devmajor=%s  ", tarfs_ptr->devmajor);
  kprintf("devminor=%s  ", tarfs_ptr->devminor);
  kprintf("size=%d ", file_size);
  kprintf("typeflag=%c ", file_type);
  //kprintf("header=%p", file_tarfs_header);
  kprintf("\n");*/

  int parent_index = 0;
  switch(file_type) {
    case TYPE_FILE:;
      parent_index = get_parent_index(file_name, 1);
      break;
    case TYPE_DIRECTORY:;
      contents = NULL;
      parent_index = get_parent_index(file_name, 2);
      break;
  }

  vfs_insert(vfs_size++, file_name, parent_index, file_size, file_type, address, offset, contents);
}

/***
 * Initialize the virtual file system by reading contents from the tarfs.
 */
void t_init_tarfs() {

  vfs_insert(0, "/", -1, 0, TYPE_DIRECTORY, 0, 0, NULL);
  vfs_insert(1, "/rootfs/", 0, 0, TYPE_DIRECTORY, 0, 512, NULL);
  vfs_size = 2;

  int offset = 512;
  struct posix_header_ustar *tarfs_ptr = (struct posix_header_ustar *) &_binary_tarfs_start;
  int file_number = 0;

  while(tarfs_ptr <= (struct posix_header_ustar *) &_binary_tarfs_end && strlen(tarfs_ptr->name) != 0) {
    uint64_t file_size = octal_to_binary((unsigned char *)tarfs_ptr->size, 11);
    char file_type = tarfs_ptr->typeflag[0];

    if(file_size == 0 || file_size % 512 == 0) {
      offset += file_size + 512;
    } else {
      offset += (file_size + (512 - file_size % 512) + 512);
    }

    vfs_add(tarfs_ptr, offset, (char *)((uint64_t)tarfs_ptr + sizeof(struct posix_header_ustar)));

    file_number = (((file_size + 511) / 512) + 1);

//    char *file_ptr = NULL;
    //to skip the file contents
    if(file_type == TYPE_FILE) {
      file_number++;

//      file_ptr = (char *)((uint64_t)tarfs_ptr + sizeof(struct posix_header_ustar));
      //kprintf("file_ptr = %p\n", file_ptr);
      //print file contents
      //kprintf("-------------->%c<-----\n", *((char *)((uint64_t)tarfs_ptr + sizeof(struct posix_header_ustar))));
      /*for(int k = 0; k < 10; k++) {
        kprintf("-------------->%c<-----\n", *(((char *) ((uint64_t) tarfs_ptr + sizeof(struct posix_header_ustar))) + k));
      }*/
    }

    //kprintf("next file address = %p\n", tarfs_ptr + file_number);

//    int l = 0;
    //kprintf("name=%s\n", tarfs_ptr->name);
    //while(!strcmp(tarfs_ptr->name, "/rootfs/etc/test.txt") && file_ptr != NULL && l < 15 /*&& file_ptr < (char *)tarfs_ptr*/) {
    /*if(file_type == TYPE_FILE) {
      kprintf("name=%s\n", tarfs_ptr->name);
    }
    for(int j = 0; j < octal_to_binary((unsigned char *)tarfs_ptr->size, 11); j++) {
      kprintf("%c", *file_ptr);
      file_ptr++;
//      l++;
    }
    kprintf("\n");*/

    tarfs_ptr += file_number;
  }
}

/***
 * Prints the entire contents of the virtual file system.
 */
void t_print_vfs() {
  for(int i = 0; i < vfs_size; i++) {
    kprintf("Name = %s, parent = %s, offset = %d, size = %d\n",
            vfs[i].name, vfs[vfs[i].parent].name, vfs[i].offset, vfs[i].size);
  }
}

/**
 * Gets the index of the file in the vfs
 * @param complete absolute path to the file
 * @return index in the vfs array, -1 if file not found
 */
int t_tarfs_get_index(const char *filename) {
  int index = 0;
  while(strcmp(vfs[index].name, filename) && index < vfs_size) {
    index++;
  }
  return index < vfs_size ? index : -1;
}

/**
 * Finds the children of a particular directory
 * @param vfs_indexes array where the indexes of the children will be written
 * @param vfs_indexes_size size of vfs_indexes
 * @return number of indexes filled, will be less than or equal to vfs_indexes_size
 */
int t_get_children_index(const char *name, int *vfs_indexes, int vfs_indexes_size) {
  int j = 0;
  for(int i = 0; i < vfs_size && j < vfs_indexes_size; i++) {
    if(!strcmp(vfs[vfs[i].parent].name, name)) {
      vfs_indexes[j++] = i;
    }
  }
  return j;
}

ssize_t t_read(int fd, char *buffer, uint64_t size) {
  //get current process
  pcb *current_process = p_get_current_process();
  //check if the file descriptor is present
  if(fd >= current_process->fd_array_size || !current_process->fd_array[fd].live) {
    return (ssize_t)0;
  }
  //check if the file_pointer < size of file
  if(current_process->fd_array[fd].position >= vfs[current_process->fd_array[fd].file_pointer].size) {
    return (ssize_t)0;
  }
  //take min(size, remaining file) and put it in the buffer
  int previous_position = current_process->fd_array[fd].position;
  int i = 0;
  int buffer_index = 0;
  for(i = current_process->fd_array[fd].position;
      i < size - 1 && i < vfs[current_process->fd_array[fd].file_pointer].size;
      i++, buffer_index++) {
    buffer[buffer_index] = *(vfs[current_process->fd_array[fd].file_pointer].contents + i);
  }
  buffer[buffer_index] = '\0';

  //update the file_pointer
  current_process->fd_array[fd].position = i;

  //return the value min(size, remaining file)
  return (ssize_t)(i - previous_position);
}