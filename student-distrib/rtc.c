#include "rtc.h"

static volatile int readFlag = 0;

/*
* _
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
    // UNCOMMENT "printf("test")" TO SEE RTC_WRITE TEST WORK
    readFlag = 1;
    // printf("test ");

    // test_interrupts();
        // send_eoi(RTC_IRQ);


   /* in order to leave RTC in usable (able to send more interrupts) state 
      after handling the current interrupt, must read register C */
    outb(REGISTERC, RTCPORT);
    inb(RTCDATA);

    // readFlag = 0;
    send_eoi(RTC_IRQ);
}

/*
* change_rate 
*   DESCRIPTION: Changes the frequency at which the RTC operates could help with testing and stuff
*   INPUTS:             rate - rtc_write() passes in some rate that is a power of 2, this value is used in setting new frequency
*   OUTPUTS:            none
*   RETURN VALUE:       none
*   SIDE EFFECTS:       Changes the frequency rate of the RTC
*
*/
void change_rate(uint32_t rate) {
    uint32_t rateCounter = 0;

    // programs should not be able to  set frequency to anything over 1024
    // rate &= FREQ_RATE_MASK;
    if (rate < DEFAULT_FREQ || rate > FREQ_UPPER_BOUND)
        return;

    // calculate log base 2 of rate, store answer in rateCounter
    while (rate != 1) {
        rateCounter++;
        rate = rate >> 1;
    }

    // 16 is number used in formula to calculate rate with which a frequency is found and set to
    // formula being 16 - log base 2 of rate as derived from page 19 of new RTC datasheet
    rate = 16 - rateCounter;

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

/*
* rtc_open()
*   DESCRIPTION: inializes RTC frequency to 2 HZ, and returns 0 for success
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   0 for success
*   SIDE EFFECTS:   none
*/
uint32_t rtc_open(uint32_t fileDesc) {
    // done
    change_rate(DEFAULT_FREQ);
    return 0;
}

/*
* rtc_close()
*   DESCRIPTION: Does pretty much nothing for now because virtualization not complete
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   0 for success
*   SIDE EFFECTS:   none
*/
uint32_t rtc_close(uint32_t fileDesc) {
    // done
    return 0;
}


/*
* rtc_read()
*   DESCRIPTION: Should block until the next interrupt occurs
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   0 on success, which is when an interrupt occurs
*   SIDE EFFECTS:   none
*  
*/
uint32_t rtc_read(uint32_t fileDesc) {
    /* volatile variable allows us to know when to read
   only executes read function when new interrupt has occurred */

   // donzo
    while(!readFlag){}
    readFlag = 0;
    return 0;
}

/*
* rtc_write()
*   DESCRIPTION: Changes the frequency, must be to a power of two
*   INPUTS:         buf - frequency to change to 
*   OUTPUTS:        
*   RETURN VALUE:   0 if valid frequency and freq changed successfully
*                   -1 if failure (invalid inputs)
*   SIDE EFFECTS:   none
*/
uint32_t rtc_write(uint32_t fileDesc,  uint32_t * buf) {
    uint32_t rate;

    // make sure arguments are valid 
    if (buf == NULL)
        return -1;
    
    rate = *buf;

    // rate program wants to set rtc to must be valid
    if (rate > FREQ_UPPER_BOUND || rate < DEFAULT_FREQ)
        return -1;

    /* for sake of rtc_write(), rate must be power of 2 */
    // so if that calculation is equal to 0, rate is power of 2
    if ((rate & (rate - 1)) != 0)         
        return -1;

    change_rate(rate);

    return 0;
}
