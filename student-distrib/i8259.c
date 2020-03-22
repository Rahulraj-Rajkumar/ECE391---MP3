/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask = 0xFF; /* IRQs 0-7  */
uint8_t slave_mask = 0xFF;  /* IRQs 8-15 */

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
* i8259_init
*   DESCRIPTION: Initialize the 8259 master and slave PICs
*   INPUTS:         NONE
*   OUTPUTS:        NONE    
*   RETURN VALUE:   NONE
*   SIDE EFFECTS:   Writes initialization control words to address and data ports of master/slave PICs, 
*                   enables interrupts to be received from slave via IR port #2 on master.
*
*/
void i8259_init(void) {

    /* mask interrupts on master and slave PICs */
    outb(INIT_MASK, MASTER_DATA);
    outb(INIT_MASK, SLAVE_DATA);

    /* send control word 1 */
    /* Tells PIC its being initialized, it should use edge-triggered input signals, and it is operating in cascade mode */
    outb(ICW1, MASTER_8259_PORT);
    outb(ICW1, SLAVE_8259_PORT);
    
    /* send control word 2 */
    outb(ICW2_MASTER, MASTER_DATA);
    outb(ICW2_SLAVE, SLAVE_DATA);

    /* control word 3 */
    outb(ICW3_MASTER, MASTER_DATA);
    outb(ICW3_SLAVE, SLAVE_DATA);

    /* control word 4 */
    outb(ICW4, MASTER_DATA);
    outb(ICW4, SLAVE_DATA);


    /* enable irq's to be received from the slave through IR port two on master */
    enable_irq(ICW3_SLAVE);

}


/*
* enable_irq
*   DESCRIPTION: Enable (unmask (set to 0)) the specified IRQ
*   INPUTS:         irq_num - the irq number to unmask (0 - 15)
*   OUTPUTS:        NONE
*   RETURN VALUE:   NONE
*   SIDE EFFECTS:   unmasks the irq at the given IRQ number
*
*/
void enable_irq(uint32_t irq_num) {
    uint32_t bit_vector = 1;
    uint32_t destination = MASTER_DATA;
    uint32_t isSlave = 0;

    /* check if irq_num is valid */
    if (irq_num > MAX_IRQ_NUM || irq_num < 0) return;

    /* if irq_num is above 7, num is referring to IRQ of slave */
    if (irq_num > MASTER_IRQ) { 
        irq_num -= (MASTER_IRQ + 1);
        destination = SLAVE_DATA; 
        isSlave = 1;
    }

    /* set up mask bit vector such that the existing mask is combined with the newly unmasked IRQ */
    bit_vector = bit_vector << irq_num;
    bit_vector = ~bit_vector;

    if (isSlave) {
        slave_mask &= bit_vector;
        outb(slave_mask, destination);
    } else {
        master_mask &= bit_vector;
        outb(master_mask, destination);
    }

}


/*
* disable_irq
*   DESCRIPTION: disable (mask (set to 1)) the specified IRQ
*   INPUTS:         irq_num - the irq number to mask (0 - 15)
*   OUTPUTS:        NONE
*   RETURN VALUE:   NONE
*   SIDE EFFECTS:   masks the irq at the given IRQ number
*
*/
void disable_irq(uint32_t irq_num) {
    uint32_t bit_vector = 1;
    uint32_t destination = MASTER_DATA;
    uint32_t isSlave = 0;

    /* check if irq_num is valid */
    if (irq_num > MAX_IRQ_NUM || irq_num < 0) return;

    /* if irq_num is above 7, num is referring to IRQ of slave */
    if (irq_num > MASTER_IRQ) { 
        irq_num -= (MASTER_IRQ + 1);
        destination = SLAVE_DATA; 
        isSlave = 1;
    }

    /* set up mask bit vector such that the existing mask is combined with the newly unmasked IRQ */
    bit_vector = bit_vector << irq_num;
    

    if (isSlave) {
        slave_mask |= bit_vector;
        outb(slave_mask, destination);
    } else {
        master_mask |= bit_vector;
        outb(master_mask, destination);
    }

}


/*
* send_eoi
*   DESCRIPTION: Send end-of-interrupt signal for the specified IRQ
*   INPUTS:         irq_num - the number of the interrupt that has just been handled
*   OUTPUTS:        NONE
*   RETURN VALUE:   NONE
*   SIDE EFFECTS:   Lets PIC know that the interrupt for irq_num has been handled, and thus sets the PIC mask appropriately
*
*/
void send_eoi(uint32_t irq_num) {
    uint32_t destination = MASTER_8259_PORT;
    uint32_t isSlave = 0;

    /* check if irq_num is valid */
    if (irq_num > MAX_IRQ_NUM || irq_num < 0) return;

    /* if irq_num is above 7, num is referring to IRQ of slave */
    if (irq_num > MASTER_IRQ) { 
        irq_num -= (MASTER_IRQ + 1);
        destination = SLAVE_8259_PORT;
        isSlave = 1;
    }

    outb(irq_num | EOI, destination);

    /* if EOI is for IRQ on slave chip, must update pin on master as well */
    if (isSlave)
        outb(ICW3_SLAVE | EOI, MASTER_8259_PORT);

}
