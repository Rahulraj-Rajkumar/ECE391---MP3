#include "rtc.h"

/*
* ___
*   DESCRIPTION:
*   INPUTS: 
*   OUTPUTS: 
*   RETURN VALUE:
*   SIDE EFFECTS: 
*
*/


/*
* initialize_rtc
*   DESCRIPTION: Sets up RTC device so it is ready to generate and send interrupts
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   none
*   SIDE EFFECTS:   Sends data to RTC to initialize it
*
*/
void initialize_rtc() {
    disable_irq(RTC_IRQ);

    /* we gonna turn on periodic interrupts on the RTC */
    outb(REGISTERB, RTCPORT);                   // selects register B as the register we want to mess around with
    char regB = inb(RTCDATA);                   // saves the current value of selected register                                          
    outb(REGISTERB, RTCPORT);                   // resets selected register
    outb(regB | REGISTER_B_CONSTANT, RTCDATA);   // resets register to modified version of regB


    enable_irq(RTC_IRQ);

}



/*
* rtc_int
*   DESCRIPTION: Handler kernel uses to respond to interrupt receieved by RTC
*   INPUTS:             none
*   OUTPUTS:            none    
*   RETURN VALUE:       none
*   SIDE EFFECTS:       Takes appropriate action for some interrupt receieved by RTC, and ensures RTC and pic is left in proper state
*
*/
void rtc_int()
{
    // test_interrupts();
    send_eoi(RTC_IRQ);

   /* in order to leave RTC in usable (able to send more interrupts) state 
      after handling the current interrupt, must read register C */
    outb(REGISTERC, RTCPORT);
    inb(RTCDATA);
}

/*
* change_rate 
*   DESCRIPTION: Changes the frequency at which the RTC operates could help with testing and stuff
*   INPUTS:             none
*   OUTPUTS:            none
*   RETURN VALUE:       none
*   SIDE EFFECTS:       Changes the frequency rate of the RTC
*
*/
void change_rate(uint32_t rate) {

    // rate must be above 2 and below 16 so frequency value of interrupts will not "roll over"
    rate &= FREQ_RATE_MASK;
    if (rate <= 2 || rate >= 16)
        return;

    disable_irq(RTC_IRQ);
    cli();

    // select register A, save current val, reselect register A, write new
    // frequency to register in terms of rate and previous register val
    outb(A_NO_NMI, RTCPORT);
    char temp = inb(RTCDATA);
    outb(A_NO_NMI, RTCPORT);
    outb(((temp & CUR_MASK) | rate), RTCDATA);

    sti();
    enable_irq(RTC_IRQ);

}
