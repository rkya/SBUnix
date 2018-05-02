#ifndef _MM_H
#define _MM_H
#include <sys/defs.h>
#include <sys/paging.h>

#define VMA_TYPE_GENERAL 1
#define VMA_TYPE_TEXT 2
#define VMA_TYPE_DATA 3

/*
	Structure refereneces:
	https://notes.shichao.io/lkd/ch15/
*/
typedef struct mm_struct {
  struct vm_area_struct *vma_head, *vma_tail;
  int map_count;
  uint64_t elf_vaddr_start, elf_vaddr_end;
  int mm_struct_type;
  int access_right;
} mm_struct;

typedef struct vm_area_struct {
  struct mm_struct *mm_struct;
  uint64_t vma_start, vma_end;
  struct vm_area_struct *next_vma;
  uint64_t file_ptr;
  uint64_t file_offset;
  uint64_t file_size;
  uint64_t file_bss_size;
  uint64_t flags;
  uint64_t offset;
  uint64_t mmsz;
  int type;
} vm_area_struct;

void memset(void *, int, uint64_t);
void memcpy(void *, void *, uint64_t);
//struct vm_area_struct * create_vma();

inline void set_cr3(pml4 *pml4_t) {
  __asm__ volatile("mov %0, %%cr3"::"b"(pml4_t));
}

#endif
