#ifndef _INT_WRAPPER_H
#define _INT_WRAPPER_H


#ifndef ASM
    extern void pit_int_w();
    extern void rtc_int_w();
    extern void kbrd_int_w();
    extern void sys_int_w();
#endif

#endif
