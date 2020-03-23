#include "paging.h"
#include "types.h"

uint32_t page_directory[PAGE_SIZE] __attribute__((aligned (PAGE_SIZE)));
uint32_t page_table[PAGE_SIZE] __attribute__((aligned (PAGE_SIZE)));


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

    for(i = 0; i < TABLE_SIZE; i++){
        page_table[i] = ((i == VIDEO_LOC/PAGE_SIZE)) ? ((VIDEO_LOC) | (DEFAULT_PTE)) : ((DEFAULT_PTE ^ PTE_P));
    }

    page_directory[0] = (uint32_t) page_table | PDE_RW | PDE_P;

    page_directory[1] = KERNEL_LOC | PDE_PS | DEFAULT_PTE;

    for(i = 2; i < TABLE_SIZE; i++){
        page_directory[i] = (i << PDE_4MP_PBA_OFFSET) | PDE_PS | PDE_RW;
    }

	asm volatile("					\n\
		movl	%%cr4, %%eax		\n\
		orl		$0x10, %%eax		\n\
		movl	%%eax, %%cr4		\n\
		movl	$page_directory, %%eax			\n\
		movl	%%eax, %%cr3		\n\
		movl	%%cr0, %%eax		\n\
		orl		$0x80000000, %%eax	\n\
		movl	%%eax, %%cr0		\n\
		"
		:
		:
		: "eax"
	);

}
