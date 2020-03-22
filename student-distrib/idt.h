#ifndef _IDT_H
#define _IDT_H

#include "x86_desc.h"
#include "ex_wrapper.h"
#include "int_wrapper.h"

void idt_setup();
void initialize_idt();


#define INT_KBRD 0x21
#define INT_RTC 0x28
#define INT_SYSCALL 0x80

#endif
