#ifndef _EX_WRAPPER_H
#define _EX_WRAPPER_H


#ifndef ASM
extern void divide_error_ex_w();
extern void debug_ex_w();
extern void nmi_int_ex_w();
extern void breakpoint_ex_w();
extern void overflow_ex_w();
extern void bound_range_exceeded_ex_w();
extern void opcode_invalid_ex_w();
extern void no_device_ex_w();
extern void double_fault_ex_w();
extern void coprocessor_segment_overrun_ex_w();
extern void tss_invalid_ex_w();
extern void no_segment_ex_w(); 
extern void stack_fault_ex_w();
extern void gp_fault_ex_w();
extern void page_fault_ex_w();
extern void fpu_ex_w();
extern void alignment_check_ex_w();
extern void machine_check_ex_w();
extern void simd_ex_w();
#endif



#endif
