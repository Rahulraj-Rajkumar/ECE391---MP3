#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "lib.h"
#include "i8259.h"

#define BLANK_SCANCODE  0x00
#define ESC_SCANCODE  0x1B
#define SCANCODE_LENGTH 0x80
#define KBRD_IRQ 0x01
#define KBRD_PORT 0x60

char kbrdscancode[SCANCODE_LENGTH];

void kbrd_init();
void kbrd_int();



#endif
