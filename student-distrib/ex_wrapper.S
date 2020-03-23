#define ASM     1
#include "ex_wrapper.h"

.text

.global divide_error_ex_w, debug_ex_w, nmi_int_ex_w, breakpoint_ex_w, overflow_ex_w, bound_range_exceeded_ex_w, opcode_invalid_ex_w, no_device_ex_w, double_fault_ex_w, coprocessor_segment_overrun_ex_w, tss_invalid_ex_w, no_segment_ex_w,  stack_fault_ex_w, gp_fault_ex_w, page_fault_ex_w, fpu_ex_w, alignment_check_ex_w, machine_check_ex_w, simd_ex_w, rtc_int_w, kbrd_int_w


divide_error_ex_w:
    pushal
    pushfl
    call divide_error_ex
    popfl
    popal
    iret

debug_ex_w:
    pushal
    pushfl
    call debug_ex
    popfl
    popal
    iret

nmi_int_ex_w:
    pushal
    pushfl
    call nmi_int_ex
    popfl
    popal
    iret

breakpoint_ex_w:
    pushal
    pushfl
    call breakpoint_ex
    popfl
    popal
    iret

overflow_ex_w:
    pushal
    pushfl
    call overflow_ex
    popfl
    popal
    iret

bound_range_exceeded_ex_w:
    pushal
    pushfl
    call bound_range_exceeded_ex
    popfl
    popal
    iret

opcode_invalid_ex_w:
    pushal
    pushfl
    call opcode_invalid_ex
    popfl
    popal
    iret

no_device_ex_w:
    pushal
    pushfl
    call no_device_ex
    popfl
    popal
    iret

double_fault_ex_w:
    pushal
    pushfl
    call double_fault_ex
    popfl
    popal
    iret

coprocessor_segment_overrun_ex_w:
    pushal
    pushfl
    call coprocessor_segment_overrun_ex
    popfl
    popal
    iret

tss_invalid_ex_w:
    pushal
    pushfl
    call tss_invalid_ex
    popfl
    popal
    iret

no_segment_ex_w: 
    pushal
    pushfl
    call no_segment_ex
    popfl
    popal
    iret

stack_fault_ex_w:
    pushal
    pushfl
    call stack_fault_ex
    popfl
    popal
    iret

gp_fault_ex_w:
    pushal
    pushfl
    call gp_fault_ex
    popfl
    popal
    iret

page_fault_ex_w:
    pushal
    pushfl
    call page_fault_ex
    popfl
    popal
    iret

fpu_ex_w:
    pushal
    pushfl
    call fpu_ex
    popfl
    popal
    iret

alignment_check_ex_w:
    pushal
    pushfl
    call alignment_check_ex
    popfl
    popal
    iret

machine_check_ex_w:
    pushal
    pushfl
    call machine_check_ex
    popfl
    popal
    iret

simd_ex_w:
    pushal
    pushfl
    call simd_ex
    popfl
    popal
    iret

