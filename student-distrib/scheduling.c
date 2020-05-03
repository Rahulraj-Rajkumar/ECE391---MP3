#include "scheduling.h"
#include "i8259.h"

void initialize_pit() {
    disable_irq(PIT_IRQ);

    /* we gonna turn on periodic interrupts on the RTC */
    outb(PIT_MODE, MCREG);                   // selects channel 0, sets access mode 10 lobyte/hibyte, and enables mode 2                                    
    outb(FREQ_40HZ & BYTEMASK, CHANNEL_0);             // Sets the frequency to 40 HZ (108/55 * 1000000 / 40 = 29830)
    outb((FREQ_40HZ >> 8) & BYTEMASK, CHANNEL_0);   


    enable_irq(PIT_IRQ);
}

void pit_int(){
    cli();

    next_process();

    send_eoi(PIT_IRQ);

    sti();
}