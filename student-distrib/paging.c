#include "paging.h"
#include "types.h"

uint32_t page_directory[TABLE_SIZE] __attribute__((aligned (PAGE_SIZE)));
uint32_t page_table[TABLE_SIZE] __attribute__((aligned (PAGE_SIZE)));


/*
* init_paging
*   DESCRIPTION: Sets up paging 
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   none
*   SIDE EFFECTS:   Initializes paging
*
*/
void init_paging(){
    int i;

	// Initialize the page table at 0-4 MB so only the video memory page is present
    for(i = 0; i < TABLE_SIZE; i++){
        page_table[i] = (i == VIDEO_LOC/PAGE_SIZE) ? ((VIDEO_LOC) | (DEFAULT_PTE)) : ((DEFAULT_PTE) ^ (PTE_P));
    }

	// Add page table to page directory, then add 4MB kernel page to directory
    page_directory[0] = ((uint32_t) page_table | PDE_RW) | PDE_P;

    page_directory[1] = (KERNEL_LOC | PDE_PS) | DEFAULT_PTE;

	// Fill the rest of the directory with not present 4MB pages 
    for(i = 2; i < TABLE_SIZE; i++){
        page_directory[i] = (i << PDE_4MP_PBA_OFFSET) | PDE_PS | PDE_RW;
    }

	asm volatile("					\n\
		movl	%%cr4, %%eax		\n\
		orl		$0x10, %%eax		\n\
		movl	%%eax, %%cr4		\n\
		movl	%%cr0, %%eax		\n\
		orl		$0x80000000, %%eax	\n\
		movl	%%eax, %%cr0		\n\
		movl	$page_directory, %%eax			\n\
		movl	%%eax, %%cr3		\n\
		"
		:
		:
		: "eax"
	);

}
