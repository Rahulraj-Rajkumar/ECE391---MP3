#include "paging.h"
#include "types.h"

uint32_t page_directory[PAGE_SIZE] __attribute__((aligned (PAGE_SIZE)));
uint32_t vid_page_table[PAGE_SIZE] __attribute__((aligned (PAGE_SIZE)));
uint32_t user_vid_page_table[PAGE_SIZE] __attribute__((aligned (PAGE_SIZE)));


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
	//initialize video memory part of paging
    for(i = 0; i < TABLE_SIZE; i++){
        vid_page_table[i] = ((i == VIDEO_LOC/PAGE_SIZE)) ? ((VIDEO_LOC) | (DEFAULT_PTE)) : ((DEFAULT_PTE) ^ (PTE_P));
    }

	user_vid_page_table[0] = VIDEO_LOC | PTE_US | DEFAULT_PTE;

	//initialize the rest of the omb to 4mb
    page_directory[0] = (uint32_t) vid_page_table | PDE_RW | PDE_P;
	//initialize the kernel to 4mb to 8mb
    page_directory[1] = KERNEL_LOC | PDE_PS | DEFAULT_PTE;
	//set the rest to not present
    for(i = 2; i < TABLE_SIZE; i++){
        page_directory[i] = (i << PDE_4MP_PBA_OFFSET) | PDE_PS | PDE_RW;
    }

	page_directory[USER_VID_PT_INDEX] = (uint32_t) user_vid_page_table | PDE_US | PDE_RW | PDE_P;

	// Place the page directory into cr3, then set bit 4 of CR4 (PSE flag) and bit 31 of CR0 (PG flag)
	asm volatile("						\n\
		movl	$page_directory, %%eax	\n\
		movl	%%eax, %%cr3			\n\
		movl	%%cr4, %%eax			\n\
		orl		$0x10, %%eax			\n\
		movl	%%eax, %%cr4			\n\
		movl	%%cr0, %%eax			\n\
		orl		$0x80000000, %%eax		\n\
		movl	%%eax, %%cr0			\n\
		"
		:
		:
		: "eax"
	);
}

/*
* change_process
*   DESCRIPTION:  Sets up new page for new current process (user program or something) and flushes out TLB
*   INPUTS:         int pid
*   OUTPUTS:        none
*   RETURN VALUE:   none
*   SIDE EFFECTS:   flushes tlb, sets up memory for new process
*
*/
void change_process(int pid){
	// changes process and cleans out TLB
	page_directory[USER_PROCESS_INDEX] = (PROCESS_LOC(pid)) | PDE_PS | PDE_US | PDE_RW | PDE_P;
	flush_tlb();
}


/*
* flush_tlb()
*   DESCRIPTION:  Cleans out TLB for fresh memory accesses
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   none
*   SIDE EFFECTS:   resets tlb
*
*/
void flush_tlb(){
	asm volatile("						\n\
		movl	%%cr3,%%eax				\n\
		movl	%%eax,%%cr3				\n\
		"
		:
		:
		: "eax"
	);
}
