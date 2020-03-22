#include "ex_handler.h"

void divide_error_ex(){
    cli();
    printf("divide exception");
    while(1);
}   

void debug_ex(){
    cli();
    printf("debug exception");
    while(1);
}

void nmi_int_ex(){
    cli();
    printf("nmi int exception");
    while(1);
}

void breakpoint_ex(){
    cli();
    printf("breakpoint exception");
    while(1);
}

void overflow_ex(){
    cli();
    printf("overflow exception");
    while(1);
}

void bound_range_exceeded_ex(){
    cli();
    printf("bound range exceeded exception");
    while(1);
}

void opcode_invalid_ex(){
    cli();
    printf("opcode invalid exception");
    while(1);
}

void no_device_ex(){
    cli();
    printf("no device found exception");
    while(1);
}

void double_fault_ex(){
    cli();
    printf("double fault exception");
    while(1);
}

void coprocessor_segment_overrun_ex(){
    cli();
    printf("coprocessor segment overrun exception");
    while(1);
}

void tss_invalid_ex(){
    cli();
    printf("tss invalid exception");
    while(1);
}

void no_segment_ex(){
    cli();
    printf("no segment found exception");
    while(1);
} 

void stack_fault_ex(){
    cli();
    printf("stack fault exception");
    while(1);
}

void gp_fault_ex(){
    cli();
    printf("gp fault exception");
    while(1);
}

void page_fault_ex(){
    cli();
    printf("page fault exception");
    while(1);
}

void fpu_ex(){
    cli();
    printf("fpu exception");
    while(1);
}

void alignment_check_ex(){
    cli();
    printf("alignment check exception");
    while(1);
}

void machine_check_ex(){
    cli();
    printf("machine check exception");
    while(1);
}

void simd_ex(){
    cli();
    printf("simd floating point     tion");
    while(1);
}
