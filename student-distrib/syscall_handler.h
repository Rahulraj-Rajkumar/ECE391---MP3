#ifndef _SYSCALL_HANDLER_H
#define _SYSCALL_HANDLER_H

#include "lib.h"
#include "file_system.h"
#include "x86_desc.h"
#include "types.h"
#include "keyboard.h"

#define PCB_BITMASK 0xFFFFE000
#define MAX_OPEN_PROCESSES 8
#define KSTACK_SIZE 0x2000
#define NUM_FOPS 4
#define STDIN 0
#define STDOUT 1
#define KERNEL_MEM_END  0x800000
#define K_STACK_SIZE    0x2000
#define WORD_SIZE       4
#define USR_START_ADDR  0x08048000
#define ADDR_DIST_EIP   24
#define USR_STACK_ADDR  0x8400000 - WORD_SIZE
#define PCB_BITMASK     0xFFFFE000


int32_t halt(uint8_t status);
int32_t execute(const uint8_t* command);
int32_t read(int32_t fd, void* buf, int32_t nbytes);
int32_t write(int32_t fd, const void* buf, int32_t nbytes);
int32_t open(const uint8_t* filename);
int32_t close(int32_t fd);
int32_t getargs(uint8_t* buf, int32_t nbytes);
int32_t vidmap(uint8_t** screen_start);
int32_t set_handler(int32_t signum, void* handler_address);
int32_t sigreturn(void);
void user_execute_helper(int32_t process_id);

void no_func();

#endif