#include "ex_handler.h"
#include "ex_wrapper.h"
#include "x86_desc.h"

// Handlers to output text in the case of an exception and freeze kernel

void divide_error_ex(){
    int ds;
    asm("movl %%ds, %0" : "=r"(ds) :);
    cli();
    printf("divide by zero exception");
    if(ds != USER_DS)
    while(INF_LOOP);
}   

void debug_ex(){
    int ds;
    asm("movl %%ds, %0" : "=r"(ds) :);
    cli();
    printf("debug exception");
    if(ds != USER_DS)
    while(INF_LOOP);
}

void nmi_int_ex(){
    int ds;
    asm("movl %%ds, %0" : "=r"(ds) :);
    cli();
    printf("nmi int exception");
    if(ds != USER_DS)
    while(INF_LOOP);
}

void breakpoint_ex(){
    int ds;
    asm("movl %%ds, %0" : "=r"(ds) :);
    cli();
    printf("breakpoint exception");
    if(ds != USER_DS)
    while(INF_LOOP);
}

void overflow_ex(){
    int ds;
    asm("movl %%ds, %0" : "=r"(ds) :);
    cli();
    printf("overflow exception");
    if(ds != USER_DS)
    while(INF_LOOP);
}

void bound_range_exceeded_ex(){
    int ds;
    asm("movl %%ds, %0" : "=r"(ds) :);
    cli();
    printf("bound range exceeded exception");
    if(ds != USER_DS)
    while(INF_LOOP);
}

void opcode_invalid_ex(){
    int ds;
    asm("movl %%ds, %0" : "=r"(ds) :);
    cli();
    printf("opcode invalid exception");
    if(ds != USER_DS)
    while(INF_LOOP);
}

void no_device_ex(){
    int ds;
    asm("movl %%ds, %0" : "=r"(ds) :);
    cli();
    printf("no device found exception");
    if(ds != USER_DS)
    while(INF_LOOP);
}

void double_fault_ex(){
    int ds;
    asm("movl %%ds, %0" : "=r"(ds) :);
    cli();
    printf("double fault exception");
    if(ds != USER_DS)
    while(INF_LOOP);
}

void coprocessor_segment_overrun_ex(){
    int ds;
    asm("movl %%ds, %0" : "=r"(ds) :);
    cli();
    printf("coprocessor segment overrun exception");
    if(ds != USER_DS)
    while(INF_LOOP);
}

void tss_invalid_ex(){
    int ds;
    asm("movl %%ds, %0" : "=r"(ds) :);
    cli();
    printf("tss invalid exception");
    if(ds != USER_DS)
    while(INF_LOOP);
}

void no_segment_ex(){
    int ds;
    asm("movl %%ds, %0" : "=r"(ds) :);
    cli();
    printf("no segment found exception");
    if(ds != USER_DS)
    while(INF_LOOP);
} 

void stack_fault_ex(){
    int ds;
    asm("movl %%ds, %0" : "=r"(ds) :);
    cli();
    printf("stack fault exception");
    if(ds != USER_DS)
    while(INF_LOOP);
}

void gp_fault_ex(){
    int ds;
    asm("movl %%ds, %0" : "=r"(ds) :);
    cli();
    printf("gp fault exception");
    if(ds != USER_DS)
    while(INF_LOOP);
}

void page_fault_ex(){
    int ds;
    asm("movl %%ds, %0" : "=r"(ds) :);
    cli();
    printf("page fault exception");
    if(ds != USER_DS)
    while(INF_LOOP);
}

void fpu_ex(){
    int ds;
    asm("movl %%ds, %0" : "=r"(ds) :);
    cli();
    printf("fpu exception");
    if(ds != USER_DS)
    while(INF_LOOP);
}

void alignment_check_ex(){
    int ds;
    asm("movl %%ds, %0" : "=r"(ds) :);
    cli();
    printf("alignment check exception");
    if(ds != USER_DS)
    while(INF_LOOP);
}

void machine_check_ex(){
    int ds;
    asm("movl %%ds, %0" : "=r"(ds) :);
    cli();
    printf("machine check exception");
    if(ds != USER_DS)
    while(INF_LOOP);
}

void simd_ex(){
    int ds;
    asm("movl %%ds, %0" : "=r"(ds) :);
    cli();
    printf("simd floating point exception");
    if(ds != USER_DS)
    while(INF_LOOP);
}
