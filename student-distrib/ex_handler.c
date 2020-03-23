#include "ex_handler.h"
#include "ex_wrapper.h"

// Handlers to output text in the case of an exception and freeze kernel

void divide_error_ex(){
    cli();
    printf("divide by zero exception");
    while(INF_LOOP);
}   

void debug_ex(){
    cli();
    printf("debug exception");
    while(INF_LOOP);
}

void nmi_int_ex(){
    cli();
    printf("nmi int exception");
    while(INF_LOOP);
}

void breakpoint_ex(){
    cli();
    printf("breakpoint exception");
    while(INF_LOOP);
}

void overflow_ex(){
    cli();
    printf("overflow exception");
    while(INF_LOOP);
}

void bound_range_exceeded_ex(){
    cli();
    printf("bound range exceeded exception");
    while(INF_LOOP);
}

void opcode_invalid_ex(){
    cli();
    printf("opcode invalid exception");
    while(INF_LOOP);
}

void no_device_ex(){
    cli();
    printf("no device found exception");
    while(INF_LOOP);
}

void double_fault_ex(){
    cli();
    printf("double fault exception");
    while(INF_LOOP);
}

void coprocessor_segment_overrun_ex(){
    cli();
    printf("coprocessor segment overrun exception");
    while(INF_LOOP);
}

void tss_invalid_ex(){
    cli();
    printf("tss invalid exception");
    while(INF_LOOP);
}

void no_segment_ex(){
    cli();
    printf("no segment found exception");
    while(INF_LOOP);
} 

void stack_fault_ex(){
    cli();
    printf("stack fault exception");
    while(INF_LOOP);
}

void gp_fault_ex(){
    cli();
    printf("gp fault exception");
    while(INF_LOOP);
}

void page_fault_ex(){
    cli();
    printf("page fault exception");
    while(INF_LOOP);
}

void fpu_ex(){
    cli();
    printf("fpu exception");
    while(INF_LOOP);
}

void alignment_check_ex(){
    cli();
    printf("alignment check exception");
    while(INF_LOOP);
}

void machine_check_ex(){
    cli();
    printf("machine check exception");
    while(INF_LOOP);
}

void simd_ex(){
    cli();
    printf("simd floating point exception");
    while(INF_LOOP);
}
