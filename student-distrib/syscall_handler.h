#ifndef _SYSCALL_HANDLER_H
#define _SYSCALL_HANDLER_H

#include "lib.h"
#include "file_system.h"
#include "x86_desc.h"
#include "types.h"
#include "keyboard.h"
#include "rtc.h"
#include "paging.h"

#define PCB_BITMASK 0xFFFFE000
#define MAX_OPEN_PROCESSES 6
#define NUM_TERMINALS   3
#define SECOND_TERMINAL 2
#define MAX_OPEN_FILES 8
#define KSTACK_SIZE 0x2000
#define NUM_FOPS 4
#define ARGS_SIZE 1024
#define STDIN 0
#define STDOUT 1
#define K_MEM_END  0x800000
#define FOUR_MB    0x400000
#define K_STACK_SIZE    0x2000
#define WORD_SIZE       4
#define USR_START_ADDR  0x08048000
#define USER_VID_MEM    0x8400000
#define ADDR_DIST_EIP   24
#define USR_STACK_ADDR  0x8400000 - WORD_SIZE
#define PCB_BITMASK     0xFFFFE000
#define NUM_FILE_TYPES  3
#define READ_INDEX      0
#define WRITE_INDEX     1
#define OPEN_INDEX      2
#define CLOSE_INDEX     3

#define STD_FILES       2

#define SHELL_LENGTH    6
#define BUFSIZE     4

#define PUSHED_REGS_SIZE 68

#define MAX_NAME_LENGTH     32
#define SHELL_LENGTH        6

#define PCB_WRITE_BITMASK       0xFFFFE000

#define MCREG   0x43
#define PIT_MODE  0x36
#define CHANNEL_0 0x40
#define FREQ_40HZ 29830
#define BYTEMASK  0xFF
#define PIT_IRQ 0x0


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
void return_to_user(int process_id);
uint32_t get_curr_pcb_term();
void pit_int();
void initialize_pit();
int32_t execute_shells();
#endif
