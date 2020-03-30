#include "keyboard.h"
#include "int_wrapper.h"

static int read_lock = 0;
static int kbrd_buf_length = 0;
static int is_enter = 0;
static int is_shift = 0;
static int is_caps = 0;
static int is_ctrl = 0;


// keyboard buffer
static int8_t kbrd_buf[KBRD_BUF_LENGTH];
// int to keep track of location in keyboard buf
static uint8_t line_marker = 0;


int32_t terminal_open(){
  return 0;
}

int32_t terminal_close(){
  return 0;
}



int32_t terminal_read(uint8_t* buf, int32_t nbytes)
{
    if(buf == NULL)
    {
      return -1;
    } 

    read_lock = 1;
    is_enter = 0;
    while(!is_enter);
    cli();

	  memcpy(buf, kbrd_buf, nbytes);

    if(line_marker - nbytes < 0)
    {
	    memmove(kbrd_buf, kbrd_buf + nbytes, 0);
	    memset(kbrd_buf, NULL_SCANCODE, nbytes);
    }
    else
    {
      memmove(kbrd_buf, kbrd_buf + nbytes, line_marker - nbytes);
	    memset(kbrd_buf + line_marker - nbytes, NULL_SCANCODE, nbytes);
    }
	  kbrd_buf_length = line_marker - nbytes;

    sti();
    read_lock = 0;
    is_enter = 0;
    return 0;
}

int32_t terminal_write(const uint8_t* buf, int32_t nbytes)
{
    if(buf == NULL)
    {
      return -1;
    }
    int i;
    cli();
    for(i = 0; i < nbytes; i++)
    {
      putc(buf[i]);
    }

    sti();
    return 0;
}



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
    is_shift = 0;
    is_caps = 0;
    is_ctrl = 0;
    memset(kbrd_buf, NULL_SCANCODE, KBRD_BUF_LENGTH);
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
    uint8_t scancode;
    uint8_t key;

    if(inb(KBRD_PORT) & 1)
    {
      scancode = inb(KBRD_PORT_DATA);
      key = kbrdscancode[scancode];

      if(scancode == LCTRL_PRESS)
      {
        is_ctrl = 1;
      }
      else if(scancode == LCTRL_RELEASE)
      {
        is_ctrl = 0;
      }
      else if(scancode == LSHIFT_PRESS || scancode == RSHIFT_PRESS)
      {
        is_shift = 1;
      }
      else if(scancode == LSHIFT_RELEASE || scancode == RSHIFT_RELEASE)
      {
        is_shift = 0;
      }
      else if(scancode == CAPS_PRESS)
      {
          if(is_caps == 1)
          {
            is_caps = 0;
          }
          else
          {
            is_caps = 1;
          }
      }
      else if(scancode == ENTER_PRESS)
      {
          putc(key);
          line_marker++;
          kbrd_buf[line_marker] = ENTER_SCANCODE;
          is_enter = 1;
          kbrd_buf_length = 0;
          if(read_lock == 0)
          {
              memset(kbrd_buf, NULL_SCANCODE, KBRD_BUF_LENGTH);
			        line_marker = 0;
          }
      }
      else if(scancode == BACKSPACE_PRESS)
      {
          if(kbrd_buf_length > 0)
          {
              backspace_helper();
              kbrd_buf[line_marker - 1] = NULL_SCANCODE;
              kbrd_buf_length--;
              line_marker--;
          }
      }
      else if((kbrd_buf_length < KBRD_BUF_LENGTH -2) && scancode < SCANCODE_LENGTH)
      {
        if(is_ctrl == 1 && key == L_SCANCODE)
        {
          reset();
          puts(kbrd_buf);
        }
        else
        {
            if(is_shift == 1 && is_caps == 0)
            {
                key = shiftkbrdscancode[scancode];
            }
            else if(is_shift == 1 && is_caps == 1)
            {
                key = capsshiftkbrdscancode[scancode];
            }
            else if(is_shift == 0 && is_caps == 1)
            {
                key = capskbrdscancode[scancode];
            }
            putc(key);
            kbrd_buf[line_marker] = key;
            line_marker++;
            kbrd_buf_length++;
        }
      }
    }

    send_eoi(KBRD_IRQ);
}

