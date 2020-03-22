#ifndef _EX_HANDLER_H
#define _EX_HANDLER_H

#include "lib.h"

void divide_error_ex();
void debug_ex();
void nmi_int_ex();
void breakpoint_ex();
void overflow_ex();
void bound_range_exceeded_ex();
void opcode_invalid_ex();
void no_device_ex();
void double_fault_ex();
void coprocessor_segment_overrun_ex();
void tss_invalid_ex();
void no_segment_ex(); 
void stack_fault_ex();
void gp_fault_ex();
void page_fault_ex();
void fpu_ex();
void alignment_check_ex();
void machine_check_ex();
void simd_ex();



#endif
