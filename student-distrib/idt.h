#ifndef _IDT_H
#define _IDT_H

#include "x86_desc.h"
#include "ex_wrapper.h"
#include "int_wrapper.h"
#include "ex_handler.h"

#ifndef ASM
void idt_setup();
void initialize_idt();
#endif

#define INT_PIT 0x20
#define INT_KBRD 0x21
#define INT_RTC 0x28
#define INT_SYSCALL 0x80
#define CP1_IDTSIZE 20

#endif
