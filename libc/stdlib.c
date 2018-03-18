#include <stdlib.h>
#include <sys/paging.h>
#include <sys/mm.h>

void *malloc(size_t size) {
  //TODO: instead of calling kmalloc, implement a code which will book user level pages.
  void *new_pages = (void *) kmalloc(size);
  return new_pages;
}

void free(void *ptr) {
  //TODO: Free the allocated page and add it to the free list.
}

void *realloc(void *ptr , size_t size) {
  void *new_pages = (void *) kmalloc(size);
  memcpy(new_pages, ptr, size);
  return new_pages;
}

void exit(int status) {
  //TODO: implement.
}