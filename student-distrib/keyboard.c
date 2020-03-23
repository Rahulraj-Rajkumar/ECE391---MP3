#include "keyboard.h"
#include "int_wrapper.h"


// Scancode conversion table to take in scancode and convert it to proper outputted character value
char kbrdscancode[SCANCODE_LENGTH] = {BLANK_SCANCODE, ESC_SCANCODE, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 
	BLANK_SCANCODE,BLANK_SCANCODE,
	'q','w','e','r','t','y','u','i','o','p','[',']', BLANK_SCANCODE,BLANK_SCANCODE,
	'a','s','d','f','g','h','j','k','l',';','\'', '`', BLANK_SCANCODE,
	'\\','z','x','c','v','b','n','m',',','.','/', BLANK_SCANCODE, 
	'*', BLANK_SCANCODE, ' ', BLANK_SCANCODE, BLANK_SCANCODE,BLANK_SCANCODE,
    BLANK_SCANCODE,BLANK_SCANCODE,BLANK_SCANCODE,BLANK_SCANCODE,BLANK_SCANCODE,
    BLANK_SCANCODE,BLANK_SCANCODE,BLANK_SCANCODE,BLANK_SCANCODE,BLANK_SCANCODE,
    BLANK_SCANCODE,BLANK_SCANCODE,BLANK_SCANCODE,'-', BLANK_SCANCODE,
    BLANK_SCANCODE,BLANK_SCANCODE,'+',BLANK_SCANCODE,BLANK_SCANCODE,BLANK_SCANCODE,
    BLANK_SCANCODE,BLANK_SCANCODE,BLANK_SCANCODE,BLANK_SCANCODE,BLANK_SCANCODE,BLANK_SCANCODE,
    BLANK_SCANCODE,BLANK_SCANCODE, 
};

/*
* kbrd_init
*   DESCRIPTION: Sets up Keyboard so it is ready to generate and send interrupts
*   INPUTS:         none
*   OUTPUTS:        none
*   RETURN VALUE:   none
*   SIDE EFFECTS:   Sends data to Keyboard to initialize it
*
*/
void kbrd_init() {
    enable_irq(KBRD_IRQ);
}


/*
* kbrd_int
*   DESCRIPTION: Handler kernel uses to respond to interrupt receieved by keyboard
*   INPUTS:             none
*   OUTPUTS:            none    
*   RETURN VALUE:       none
*   SIDE EFFECTS:       Takes appropriate action for some interrupt receieved by Keyboard, and ensures keyboard is left in proper state
*
*/
void kbrd_int() {
    uint8_t key = inb(KBRD_PORT);

    if(key < SCANCODE_LENGTH) {
        putc(kbrdscancode[key]);
    }

    send_eoi(KBRD_IRQ);
}

