#ifndef _PAGING_H
#define _PAGING_H

//References: https://wiki.osdev.org/Page_Tables
//https://os.phil-opp.com/page-tables/
//http://www.noteblok.net/wp-content/uploads/sites/3/2015/01/Self-referenced_Page_Tables-Vogel-ASPLOS_SrC.pdf
//https://wiki.osdev.org/Paging

#include <sys/defs.h>
#include <sys/mm.h>
#include <sys/terminal.h>

#define ALLOCATED 1
#define FREE 0
#define PAGESIZE 4096
#define PML4SHIFT 39
#define PDPSHIFT 30
#define PDSHIFT 21
#define PTSHIFT 12
#define PAMASK 0xFFF
#define PHYMASK 0xfffffffffffff000
#define KERNAL_PAGE 0x3
#define USER_PAGE 0x7
#define USER_ACCESSIBLE 1
#define KERNEL_ACCESSIBLE 0

typedef struct Page{
  struct Page *nxtPage;
  int status; //0 - free and 1 - allocated
  uint64_t b_addr;
} Page;


//void print_next_free();
uint64_t kmalloc(uint64_t );
pml4 *p_init_paging(uint64_t , uint64_t , uint64_t );

#endif
