#define ASM     1
#include "int_wrapper.h"

.text

.global rtc_int_w, kbrd_int_w, sys_int_w

rtc_int_w:
    pushal
    pushfl
    call rtc_int
    popfl
    popal
    iret

kbrd_int_w:
    pushal
    pushfl
    call kbrd_int
    popfl
    popal
    iret

sys_int_w:
    pushal
    pushfl
    call sys_int
    popfl
    popal
    iret
