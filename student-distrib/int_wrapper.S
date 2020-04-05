#define ASM     1
#include "int_wrapper.h"

.text

.global rtc_int_w, kbrd_int_w, sys_int_w
.extern halt, execute, read, write, open, close, getargs, vidmap, set_handler, sigreturn

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
    cmpl    $10, %eax
    ja      syscall_error

    pushl   %esi
    pushl   %edi
    pushl   %ebx
    pushl   %ecx
    pushl   %edx
    pushfl

    pushl   %edx
    pushl   %ecx
    pushl   %ebx

    cmpl    $0, %eax
    call    *sys_int_table(, %eax, 4)

    popl    %ebx
    popl    %ecx
    popl    %edx

    popfl
    popl    %edx
    popl    %ecx
    popl    %ebx
    popl    %edi
    popl    %esi
    iret

syscall_error:
    movl    $-1, %eax
    iret

sys_int_table:
	.long halt, execute, read, write, open, close, getargs, vidmap, set_handler, sigreturn
