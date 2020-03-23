#include "idt.h"

/*
*   setup_idt()
*   Inputs: none
*   Return Value: none
*   Function: sets up interrupt gates for idt
*/
void setup_idt()
{
    int i;
    for (i = 0; i < NUM_VEC; i++)
    {
        if(i < CP1_IDTSIZE || i == INT_KBRD || i == INT_RTC || i == INT_SYSCALL)
        {
            idt[i].seg_selector = KERNEL_CS;
	        idt[i].reserved4 	= 0x00;
	        idt[i].reserved3 	= 0;
	        idt[i].reserved2 	= 1;
	        idt[i].reserved1 	= 1;
	        idt[i].size 		= 1;	
	        idt[i].reserved0	= 0;
	        if(i == INT_SYSCALL)
	        	idt[i].dpl 	= 3;
	        else
	        	idt[i].dpl 	= 0;
	        idt[i].present 		= 1;
        }
        else
        {
        idt[i].present = 0;
        idt[i].size = 1;
        idt[i].dpl = 0;                 
        idt[i].reserved0 = 0;
        idt[i].reserved1 = 1;
        idt[i].reserved2 = 1;
        idt[i].reserved3 = 1;
        idt[i].reserved4 = 0;     
        idt[i].seg_selector = KERNEL_CS; 
        }
    }
}

/*
*   initialize_idt()
*   Inputs: none
*   Return Value: none
*   Function: sets up defined interrupts and rtc, keyboard and syscalls
*/
void initialize_idt()
{
    SET_IDT_ENTRY(idt[0x00], divide_error_ex_w);
    SET_IDT_ENTRY(idt[0x01], debug_ex_w);
    SET_IDT_ENTRY(idt[0x02], nmi_int_ex_w);
    SET_IDT_ENTRY(idt[0x03], breakpoint_ex_w);
    SET_IDT_ENTRY(idt[0x04], overflow_ex_w);
    SET_IDT_ENTRY(idt[0x05], bound_range_exceeded_ex_w);
    SET_IDT_ENTRY(idt[0x06], opcode_invalid_ex_w);
    SET_IDT_ENTRY(idt[0x07], no_device_ex_w);
    SET_IDT_ENTRY(idt[0x08], double_fault_ex_w);
    SET_IDT_ENTRY(idt[0x09], coprocessor_segment_overrun_ex_w);
    SET_IDT_ENTRY(idt[0x0A], tss_invalid_ex_w);
    SET_IDT_ENTRY(idt[0x0B], no_segment_ex_w);
    SET_IDT_ENTRY(idt[0x0C], stack_fault_ex_w);
    SET_IDT_ENTRY(idt[0x0D], gp_fault_ex_w);
    SET_IDT_ENTRY(idt[0x0E], page_fault_ex_w);
    SET_IDT_ENTRY(idt[0x10], fpu_ex_w);
    SET_IDT_ENTRY(idt[0x11], alignment_check_ex_w);
    SET_IDT_ENTRY(idt[0x12], machine_check_ex_w);
    SET_IDT_ENTRY(idt[0x13], simd_ex_w);
    SET_IDT_ENTRY(idt[INT_KBRD], kbrd_int_w);
    SET_IDT_ENTRY(idt[INT_RTC], rtc_int_w);
    SET_IDT_ENTRY(idt[INT_SYSCALL], sys_int_w);
}
