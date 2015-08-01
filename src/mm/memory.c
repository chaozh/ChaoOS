#include <types.h>
#include <x86.h>

#include <mmu.h>
#include <page.h>

struct page* alloc_page() {

}

void free_page(struct page *page){

}
/*
 * Initialze the Page Directory 
 * called in mm_init
 */
void pgd_init() {
    
}

void page_alloc_init(){

}

void mm_init() {
    //init page directory
    pgd_init();
    //init physical memory page allocator
    page_alloc_init();
    //kmem_cache_init
    //pgtable_cache_init
    //fault handler
    
    //load page directory and enable mmu
    lpgd();
    //mmu_enable();
    //then vmalloc init
}
