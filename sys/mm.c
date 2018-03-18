#include <sys/mm.h>

/***
 * Set the memory with the specified value.
 * @param ptr starting address of memory
 * @param val to be copied
 * @param size upto which value is to be copied
 */
void memset(void *ptr, int val, uint64_t size){
  char *p = (char *)ptr;
  while(size--){
    *p = val;
    p++;
  }
}

/***
 * Copy contents from one memory location to other.
 * @param dest address where content is to be copied
 * @param source address from where content is to be copied
 * @param size of memory to be copied
 */
void memcpy(void *dest, void *source, uint64_t size) {
  char *s = (char *)source;
  char *d = (char *)dest;
  while(size--) {
    *d++ = *s++;
  }
}