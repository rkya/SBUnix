#include <sys/paging.h>
#include <sys/kprintf.h>

//References: https://wiki.osdev.org/Page_Tables
//http://www.noteblok.net/wp-content/uploads/sites/3/2015/01/Self-referenced_Page_Tables-Vogel-ASPLOS_SrC.pdf
//https://wiki.osdev.org/Paging

Page *free_list_head;
Page *entire_list;
Page *el_end; //entire list end
//uint64_t start_addr_of_fl = 0;
static pml4 *kernel_pml4 = 0;

void create_free_list(uint64_t );
uint64_t get_next_free_page();
void create_frames(uint64_t , uint64_t );
void page_table_walk(uint64_t, uint64_t, uint64_t, pml4 *, int);
void pml4_entry(uint64_t, uint64_t, pml4*, int);
void pdpt_entry(uint64_t, uint64_t , pdp*, int);
void pd_entry(uint64_t, uint64_t , pd*, int);
void pt_entry(uint64_t, uint64_t , pt*, int);
uint64_t get_offset(uint64_t , uint64_t );
void print_va_to_pa(uint64_t ,pml4*);


/***
 * Get offset based on the type of the shift.
 * @param addr whose offset is to be returned
 * @param shift to be performed
 * @return offset
 */
uint64_t get_offset(uint64_t addr, uint64_t shift){
  return (addr >> shift) & 0x1FF;
}

/***
 * Initialize paging and set the cr3.
 * @param physfree Physfree
 * @param end Ending physical address
 * @return pml4 pointer
 */
pml4 *p_init_paging(uint64_t physbase, uint64_t physfree, uint64_t end) {
  create_frames(physfree, end);
  pml4 *pml4_t = (pml4 *)get_next_free_page();
  memset((void *) pml4_t, 0, PAGESIZE);
  //using self-referencing trick
  //pml4_t[511] = (pml4)((uint64_t)pml4_t + (uint64_t)KERNBASE);

  //map kernel
  //map pages from physfree to the start of the free list in page tables
  page_table_walk(KERNBASE + physbase, KERNBASE + (uint64_t) end,
                  (uint64_t) physbase, pml4_t, USER_ACCESSIBLE);
//  page_table_walk(KERNBASE + physbase, KERNBASE + (uint64_t)free_list_head, (uint64_t) physbase, pml4_t);

  //map video card
  //map pages from (8000)_16 to (8000)_16 + (4000)_10 bytes
  page_table_walk(KERNBASE + (uint64_t) 0xb8000, KERNBASE + (uint64_t) 0xb8FA0,
                  (uint64_t) 0xb8000, pml4_t, USER_ACCESSIBLE);
  //print_va_to_pa(KERNBASE + (uint64_t)0x201230 ,pml4_t);

  //set cr3
  set_cr3(pml4_t);

  //set new video card addresses
  t_set_new_video_card_addresses();

  kernel_pml4 = pml4_t;
  return pml4_t;
}

/***
 * Make entry into the page tables and have a walk if necessary.
 * @param start starting virtual address inclusive
 * @param end ending virtual address exclusive
 * @param paddr starting physical address
 * @param pml4_t pml4 value
 * @param page_accessibility KERNEL_ACCESSIBLE or USER_ACCESSIBLE
 */
void page_table_walk(uint64_t start, uint64_t end, uint64_t paddr, pml4 *pml4_t, int page_accessibility) {
  uint64_t num_pages = 0;
  uint64_t i;
  uint64_t size = end - start;

//  kprintf("Size for page table walk = %d\n", size);
  //size = 0x5e5c000 for kernel

  if(size % PAGESIZE == 0) {
    num_pages = size / PAGESIZE;
  } else {
    num_pages = size / PAGESIZE + 1;
  }
  //num_pages = 24,156
  for(i = 0; i < num_pages; i++) {
    pml4_entry(start + (i * PAGESIZE), paddr + (i * PAGESIZE), pml4_t, page_accessibility);
  }
}

/***
 * Divide the available physical address space into frames.
 * @param physfree start of free physical address space inclusive
 * @param end end of free physical address space exclusive
 */
void create_frames(uint64_t physfree, uint64_t end) {
  uint64_t i;
  uint64_t num_pages = 0;
  uint64_t size_list = 0;
  uint64_t n_pg_list = 0;
  uint64_t start_addr_of_fl = 0;
  Page *temp;
  Page *last = NULL;
  entire_list = (Page *) physfree;
  temp = entire_list;
  //freeListHead = 0x308000

  //creates a link list of all the pages from physfree to end
  //physfree = 0x277000, end = 0x605c000
  for(i = 0; i < end; i += PAGESIZE) {
    temp->b_addr = i;
    temp->nxtPage = temp + sizeof(Page);
    last = temp;
    temp = temp->nxtPage;
    num_pages++;
  }
  last->nxtPage = NULL;
  temp = entire_list;
  size_list = num_pages * sizeof(Page);
  if(size_list % PAGESIZE != 0) {
    n_pg_list = size_list / PAGESIZE + 1; //Rounding up
  } else {
    n_pg_list = size_list / PAGESIZE;
  }
//  kprintf("Size of page list is %d and the number of pages used to store it are %d.\n", size_list, n_pg_list);
  start_addr_of_fl = physfree + n_pg_list * PAGESIZE;
//  kprintf("The start of free pages is %p.\n", start_addr_of_fl);
  create_free_list(start_addr_of_fl);
}

/***
 * Mark the used frames as ALLOCATED and the free ones as FREE.
 * @param start_addr_of_fl start of the free list
 */
void create_free_list(uint64_t start_addr_of_fl) {
  Page *temp;
  temp = entire_list;
  int sfl_flag = 0;
  while(temp) {
    if(sfl_flag == 0) {
      //set the allocated flag of the pages that are used up by the free list
      temp->status = ALLOCATED;
      if(temp->nxtPage->b_addr == start_addr_of_fl) {
        //this is where the actual free list should start
        sfl_flag = 1;
        free_list_head = temp->nxtPage;
      }
      temp = temp->nxtPage;
    } else {
      temp->status = FREE;
      temp = temp->nxtPage;
    }
  }
//  kprintf("Free_list_head : %p.\n", free_list_head->b_addr);
}

/***
 * Return the next free page from the free list.
 * @return Free page's physical address
 */
uint64_t get_next_free_page() {
  Page *temp = free_list_head;
  if(free_list_head == NULL) {
    return -1;
  } else {
    free_list_head->status = 1;
    free_list_head = free_list_head->nxtPage;
    return temp->b_addr;
  }
}

/***
 * pml4 table entry.
 * @param vaddr
 * @param paddr
 * @param pml4_t
 * @param page_accessibility
 */
void pml4_entry(uint64_t vaddr, uint64_t paddr, pml4* pml4_t, int page_accessibility) {
  uint64_t pml4off = get_offset(vaddr, PML4SHIFT);
  pdp *pdpt;
  if(*(pml4_t + pml4off) == 0) {
    pdpt = (pdp *)get_next_free_page();
    if(*pdpt != -1) {
      //insert new pdpt at the correct offset in the pml4t
      //read/write and present bit set
      if(page_accessibility == USER_ACCESSIBLE) {
        *(pml4_t + pml4off) = (uint64_t)pdpt | USER_PAGE;
      } else {
        *(pml4_t + pml4off) = (uint64_t) pdpt | KERNAL_PAGE;
      }
      memset((void *) pdpt, 0, PAGESIZE);
      pdpt_entry(vaddr, paddr, pdpt, page_accessibility);
    } else {
      kprintf("Ran out of memory\n");
    }
  } else {
    pdpt = (pdp *)(*(pml4_t + pml4off) & PHYMASK);
    pdpt_entry(vaddr, paddr, pdpt, page_accessibility);
  }
  //kprintf("Going to pdp: entry at offset is\n", *(pml4_t + pml4off));
}

/***
 * pdpt table entry.
 * @param vaddr
 * @param paddr
 * @param pdpt
 * @param page_accessibility
 */
void pdpt_entry(uint64_t vaddr, uint64_t paddr, pdp* pdpt, int page_accessibility) {
  uint64_t pdpoff = get_offset(vaddr, PDPSHIFT);
  pd *pd_t;
  if(*(pdpt + pdpoff) == 0) {
    pd_t = (pdp *)get_next_free_page();
    if(*pd_t != -1) {
      //insert new pdt at the correct offset in the pdpt
      //read/write and present bit set
      if(page_accessibility == USER_ACCESSIBLE) {
        *(pdpt + pdpoff) = (uint64_t)pd_t | USER_PAGE;
      } else {
        *(pdpt + pdpoff) = (uint64_t)pd_t | KERNAL_PAGE;
      }
      memset((void *) pd_t, 0, PAGESIZE);
      pd_entry(vaddr, paddr, pd_t, page_accessibility);
    } else {
      kprintf("Ran out of memory\n");
    }
  } else {
    pd_t = (pd *)(*(pdpt + pdpoff) & PHYMASK);
    pd_entry(vaddr, paddr, pd_t, page_accessibility);
  }
}

/***
 * pd table entry.
 * @param vaddr
 * @param paddr
 * @param pd_t
 * @param page_accessibility
 */
void pd_entry(uint64_t vaddr, uint64_t paddr, pd* pd_t, int page_accessibility) {
  uint64_t pdoff = get_offset(vaddr, PDSHIFT);
  pt *pt_t = NULL;
  if(*(pd_t + pdoff) == 0) {
    pt_t = (pdp *)get_next_free_page();
    if(*pd_t != -1) {
      //insert new pt at the correct offset in the pdt
      //read/write and present bit set
      if(page_accessibility == USER_ACCESSIBLE) {
        *(pd_t + pdoff) = (uint64_t) pt_t | USER_PAGE;
      } else {
        *(pd_t + pdoff) = (uint64_t) pt_t | KERNAL_PAGE;
      }
      memset((void *) pt_t, 0, PAGESIZE);
      pt_entry(vaddr, paddr, pt_t, page_accessibility);
    } else {
      kprintf("Ran out of memory\n");
    }
  } else {
    pt_t = (pt *)(*(pd_t + pdoff) & PHYMASK);
    pt_entry(vaddr, paddr, pt_t, page_accessibility);
  }
}

/***
 * pt table entry
 * @param vaddr
 * @param paddr
 * @param pt_t
 * @param page_accessibility
 */
void pt_entry(uint64_t vaddr, uint64_t paddr, pt* pt_t, int page_accessibility) {
  uint64_t ptoff = get_offset(vaddr, PTSHIFT);
  //finally insert new physical address in the correct offset in the pt
  //read/write and present bit set
  if(page_accessibility == USER_ACCESSIBLE) {
    *(pt_t + ptoff) = paddr | USER_PAGE;
  } else {
    *(pt_t + ptoff) = paddr | KERNAL_PAGE;
  }
}

/***
 * Print the physical address of the supplied virtual address.
 * @param vaddr whose physical address it to be printed
 * @param pml4_t
 */
void print_va_to_pa(uint64_t vaddr,pml4* pml4_t){
  pdp *pdpt;
  pd *pd_t;
  pt *pt_t;
  uint64_t paddr, pga;
  uint64_t pml4off, pdpoff, pdoff, ptoff;
  pml4off = get_offset(vaddr, PML4SHIFT);
  pdpoff = get_offset(vaddr, PDPSHIFT);
  pdoff = get_offset(vaddr, PDSHIFT);
  ptoff = get_offset(vaddr, PTSHIFT);
  pdpt = (pdp *)(*(pml4_t + pml4off) & PHYMASK);
  pd_t = (pd *)(*(pdpt + pdpoff) & PHYMASK);
  pt_t = (pt *)(*(pd_t + pdoff) & PHYMASK);
  pga = (uint64_t)(*(pt_t + ptoff) & PHYMASK);
  paddr = pga + (vaddr & PAMASK);
  kprintf("Va :%p and pa :%p\n", vaddr, paddr);
}

/***
 * Return free pages based on the size of the free space required. <br>
 * This will cause internal fragmentation if size is not a multiple of PAGESIZE.
 * @param size of memory needed
 * @return free memory >= size
 */
uint64_t kmalloc(uint64_t size){

  uint64_t flh_va = KERNBASE + (uint64_t) free_list_head;
  uint64_t num_pages;

  if(size % PAGESIZE == 0) {
    num_pages = size/PAGESIZE;
  } else {
    num_pages = size/PAGESIZE + 1;
  }
  //uint64_t upto_va = KERNBASE + (uint64_t) free_list_head + num_pages * PAGESIZE;
  //page_table_walk(flh_va, upto_va, (uint64_t) free_list_head, kernel_pml4);


  Page *temp = (Page *)flh_va;
  uint64_t ret_addr = KERNBASE + temp->b_addr;
  //uint64_t base_physical_addr = (uint64_t)free_list_head;

  /*uint64_t num_pages;
  if(size % PAGESIZE == 0){
    num_pages = size/PAGESIZE;
  }else{
    num_pages = size/PAGESIZE + 1;
  }*/
  while(num_pages--){
    temp->status = 1;
    temp = (Page *)(KERNBASE + (uint64_t)temp->nxtPage);
  }
  free_list_head = (Page*)((uint64_t)temp - KERNBASE);


  //kprintf("New flh is : %p\n", free_list_head);
  return ret_addr;
}