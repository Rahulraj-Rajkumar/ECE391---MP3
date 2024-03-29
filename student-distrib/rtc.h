#ifndef _RTC_H
#define _RTC_H

#include "lib.h"
#include "i8259.h"

#define REGISTERA                   0x0A
#define REGISTERB                   0x8B
#define REGISTERC                   0x0C
#define REGISTERD                   0x0D

#define A_NO_NMI                    0x8A

#define REGISTER_B_CONSTANT         0x40

#define RTCPORT                     0x70
#define RTCDATA                     RTCPORT + 1

#define RTC_IRQ                     8

#define FREQ_RATE_MASK              0x0F
#define CUR_MASK                    0xF0

#define DEFAULT_FREQ                2
#define FREQ_UPPER_BOUND            1024
#define POWER_OF_TWO                2

#define INITIAL_FREQ                32

void initialize_rtc();

void rtc_int();

void change_rate(uint32_t rate);
void refresh_vidmem();

int32_t rtc_open(const uint8_t* fileDesc);
int32_t rtc_close(int32_t fileDesc);
int32_t rtc_read(int32_t * fd, uint8_t* buf, int32_t fileDesc);
int32_t rtc_write(int32_t * fd, const uint8_t* buf, int32_t fileDesc);

#endif
