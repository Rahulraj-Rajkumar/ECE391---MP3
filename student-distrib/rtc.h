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

void initialize_rtc();

void rtc_int();

void change_rate(uint32_t rate);

#endif  

