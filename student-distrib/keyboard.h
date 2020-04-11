#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "lib.h"
#include "i8259.h"

#define BLANK_SCANCODE              0x00
#define SCANCODE_LENGTH             0x80
#define ESCAPE_SCANCODE             0x1B
#define KBRD_IRQ                    0x01
#define KBRD_PORT_DATA              0x60
#define KBRD_PORT                   0x64
#define KBRD_BUF_LENGTH             128


#define LCTRL_PRESS                 0x1D
#define LCTRL_RELEASE               0x9D
#define LSHIFT_PRESS                0x2A
#define RSHIFT_PRESS                0x36
#define LSHIFT_RELEASE              0xAA
#define RSHIFT_RELEASE              0xB6
#define CAPS_PRESS                  0x3A
#define BACKSPACE_PRESS             0x0E
#define TAB_PRESS                   0x0F
#define ENTER_PRESS                 0x1C

#define ZERO_SCANCODE              '0'
#define ONE_SCANCODE               '1'
#define TWO_SCANCODE               '2'
#define THREE_SCANCODE             '3'
#define FOUR_SCANCODE              '4'
#define FIVE_SCANCODE              '5'
#define SIX_SCANCODE               '6'
#define SEVEN_SCANCODE             '7'
#define EIGHT_SCANCODE             '8'
#define NINE_SCANCODE              '9'

#define ENTER_SCANCODE             '\n'
#define BACKSPACE_SCANCODE         '\b'
#define SPACE_SCANCODE             ' '
#define TAB_SCANCODE               ' '
#define NULL_SCANCODE              '\0'


#define A_SCANCODE                 'a'
#define B_SCANCODE                 'b'
#define C_SCANCODE                 'c'
#define D_SCANCODE                 'd'
#define E_SCANCODE                 'e'
#define F_SCANCODE                 'f'
#define G_SCANCODE                 'g'
#define H_SCANCODE                 'h'
#define I_SCANCODE                 'i'
#define J_SCANCODE                 'j'
#define K_SCANCODE                 'k'
#define L_SCANCODE                 'l'
#define M_SCANCODE                 'm'
#define N_SCANCODE                 'n'
#define O_SCANCODE                 'o'
#define P_SCANCODE                 'p'
#define Q_SCANCODE                 'q'
#define R_SCANCODE                 'r'
#define S_SCANCODE                 's'
#define T_SCANCODE                 't'
#define U_SCANCODE                 'u'
#define V_SCANCODE                 'v'
#define W_SCANCODE                 'w'
#define X_SCANCODE                 'x'
#define Y_SCANCODE                 'y'
#define Z_SCANCODE                 'z'

#define A_CAPS_SCANCODE            'A'
#define B_CAPS_SCANCODE            'B'
#define C_CAPS_SCANCODE            'C'
#define D_CAPS_SCANCODE            'D'
#define E_CAPS_SCANCODE            'E'
#define F_CAPS_SCANCODE            'F'
#define G_CAPS_SCANCODE            'G'
#define H_CAPS_SCANCODE            'H'
#define I_CAPS_SCANCODE            'I'
#define J_CAPS_SCANCODE            'J'
#define K_CAPS_SCANCODE            'K'
#define L_CAPS_SCANCODE            'L'
#define M_CAPS_SCANCODE            'M'
#define N_CAPS_SCANCODE            'N'
#define O_CAPS_SCANCODE            'O'
#define P_CAPS_SCANCODE            'P'
#define Q_CAPS_SCANCODE            'Q'
#define R_CAPS_SCANCODE            'R'
#define S_CAPS_SCANCODE            'S'
#define T_CAPS_SCANCODE            'T'
#define U_CAPS_SCANCODE            'U'
#define V_CAPS_SCANCODE            'V'
#define W_CAPS_SCANCODE            'W'
#define X_CAPS_SCANCODE            'X'
#define Y_CAPS_SCANCODE            'Y'
#define Z_CAPS_SCANCODE            'Z'


#define PERIOD_SCANCODE            '.'
#define COMMA_SCANCODE             ','
#define COLON_SCANCODE             ':'
#define SEMICOLON_SCANCODE         ';'
#define SLASH_SCANCODE             '/'
#define BACKSLASH_SCANCODE         '\\'
#define PLUS_SCANCODE              '+'
#define MINUS_SCANCODE             '-'
#define STAR_SCANCODE              '*'
#define EXCLAMATION_SCANCODE       '!'
#define QUESTION_SCANCODE          '?'
#define DOLLAR_SCANCODE            '$'
#define LESS_SCANCODE              '<'
#define GREATER_SCANCODE           '>'
#define LINE_SCANCODE              '|'
#define GRAVE_SCANCODE             '`'
#define TILDE_SCANCODE             '~'
#define AT_SCANCODE                '@'
#define CARET_SCANCODE             '^'
#define DOUBLEQUOTE_SCANCODE       '\"'
#define QUOTE_SCANCODE             '\''
#define EQUAL_SCANCODE             '='
#define HASH_SCANCODE              '#'
#define PERCENT_SCANCODE           '%'
#define AMPERSAND_SCANCODE         '&'
#define UNDERSCORE_SCANCODE        '_'
#define OPENPARENTHESIS_SCANCODE   '('
#define CLOSEPARENTHESIS_SCANCODE  ')'
#define OPENBRACKET_SCANCODE       '['
#define CLOSEBRACKET_SCANCODE      ']'
#define OPENCURLB_SCANCODE         '{'
#define CLOSECURLB_SCANCODE        '}'


// Scancode conversion table to take in scancode and convert it to proper outputted character value
static const uint8_t kbrdscancode[SCANCODE_LENGTH] = { 
  BLANK_SCANCODE, 
  ESCAPE_SCANCODE, 
  ONE_SCANCODE, 
  TWO_SCANCODE, 
  THREE_SCANCODE, 
  FOUR_SCANCODE, 
  FIVE_SCANCODE, 
  SIX_SCANCODE,
  SEVEN_SCANCODE, 
  EIGHT_SCANCODE, 
  NINE_SCANCODE, 
  ZERO_SCANCODE, 
  MINUS_SCANCODE, 
  EQUAL_SCANCODE, 
  BACKSPACE_SCANCODE, 
  TAB_SCANCODE,
  Q_SCANCODE,
  W_SCANCODE,
  E_SCANCODE,
  R_SCANCODE,
  T_SCANCODE,
  Y_SCANCODE,
  U_SCANCODE,
  I_SCANCODE,
  O_SCANCODE,
  P_SCANCODE,
  OPENBRACKET_SCANCODE,
  CLOSEBRACKET_SCANCODE,
  ENTER_SCANCODE,
  NULL_SCANCODE,
  A_SCANCODE,
  S_SCANCODE,
  D_SCANCODE,
  F_SCANCODE,
  G_SCANCODE,
  H_SCANCODE,
  J_SCANCODE,
  K_SCANCODE,
  L_SCANCODE,
  SEMICOLON_SCANCODE,
  QUOTE_SCANCODE,
  GRAVE_SCANCODE,
  NULL_SCANCODE,
  BACKSLASH_SCANCODE,
  Z_SCANCODE,
  X_SCANCODE,
  C_SCANCODE,
  V_SCANCODE,
  B_SCANCODE,
  N_SCANCODE,
  M_SCANCODE,
  COMMA_SCANCODE,
  PERIOD_SCANCODE,
  SLASH_SCANCODE,
  NULL_SCANCODE, 
  STAR_SCANCODE,
  NULL_SCANCODE,  // left alt
  SPACE_SCANCODE,
  NULL_SCANCODE, //f1
  NULL_SCANCODE, //f2
  NULL_SCANCODE, //f3
  NULL_SCANCODE, //f4
  NULL_SCANCODE, //f5
  NULL_SCANCODE, //f6
  NULL_SCANCODE, //f7
  NULL_SCANCODE, //f8
  NULL_SCANCODE, //f9
  NULL_SCANCODE, //f9
  NULL_SCANCODE, //f10
  NULL_SCANCODE, //numlock
  NULL_SCANCODE, //scroll lock
  NULL_SCANCODE, //home
  NULL_SCANCODE, //up
  NULL_SCANCODE, //pageup
  NULL_SCANCODE, //minus numpad 
  NULL_SCANCODE, //left
  NULL_SCANCODE, //5 numpad
  NULL_SCANCODE, //right
  NULL_SCANCODE, //plus numpad
  NULL_SCANCODE, //more numpad
  NULL_SCANCODE, //more numpad
  NULL_SCANCODE, //more numpad
  NULL_SCANCODE, //more numpad
  NULL_SCANCODE, //insert
  NULL_SCANCODE, //delete
  NULL_SCANCODE, 
  NULL_SCANCODE,
  NULL_SCANCODE, //f11
  NULL_SCANCODE, //f12
  NULL_SCANCODE, 
};

// Scancode conversion table to take in scancode and convert it to proper outputted character value with caps lock
static const uint8_t capskbrdscancode[SCANCODE_LENGTH] = { 
  BLANK_SCANCODE, 
  ESCAPE_SCANCODE, 
  ONE_SCANCODE, 
  TWO_SCANCODE, 
  THREE_SCANCODE, 
  FOUR_SCANCODE, 
  FIVE_SCANCODE, 
  SIX_SCANCODE,
  SEVEN_SCANCODE, 
  EIGHT_SCANCODE, 
  NINE_SCANCODE, 
  ZERO_SCANCODE, 
  MINUS_SCANCODE, 
  EQUAL_SCANCODE, 
  BACKSPACE_SCANCODE, 
  TAB_SCANCODE,
  Q_CAPS_SCANCODE,
  W_CAPS_SCANCODE,
  E_CAPS_SCANCODE,
  R_CAPS_SCANCODE,
  T_CAPS_SCANCODE,
  Y_CAPS_SCANCODE,
  U_CAPS_SCANCODE,
  I_CAPS_SCANCODE,
  O_CAPS_SCANCODE,
  P_CAPS_SCANCODE,
  OPENBRACKET_SCANCODE,
  CLOSEBRACKET_SCANCODE,
  ENTER_SCANCODE,
  NULL_SCANCODE,
  A_CAPS_SCANCODE,
  S_CAPS_SCANCODE,
  D_CAPS_SCANCODE,
  F_CAPS_SCANCODE,
  G_CAPS_SCANCODE,
  H_CAPS_SCANCODE,
  J_CAPS_SCANCODE,
  K_CAPS_SCANCODE,
  L_CAPS_SCANCODE,
  SEMICOLON_SCANCODE,
  QUOTE_SCANCODE,
  GRAVE_SCANCODE,
  NULL_SCANCODE,
  BACKSLASH_SCANCODE,
  Z_CAPS_SCANCODE,
  X_CAPS_SCANCODE,
  C_CAPS_SCANCODE,
  V_CAPS_SCANCODE,
  B_CAPS_SCANCODE,
  N_CAPS_SCANCODE,
  M_CAPS_SCANCODE,
  COMMA_SCANCODE,
  PERIOD_SCANCODE,
  SLASH_SCANCODE,
  NULL_SCANCODE, 
  STAR_SCANCODE,
  NULL_SCANCODE,  // left alt
  SPACE_SCANCODE,
  NULL_SCANCODE, //f1
  NULL_SCANCODE, //f2
  NULL_SCANCODE, //f3
  NULL_SCANCODE, //f4
  NULL_SCANCODE, //f5
  NULL_SCANCODE, //f6
  NULL_SCANCODE, //f7
  NULL_SCANCODE, //f8
  NULL_SCANCODE, //f9
  NULL_SCANCODE, //f9
  NULL_SCANCODE, //f10
  NULL_SCANCODE, //numlock
  NULL_SCANCODE, //scroll lock
  NULL_SCANCODE, //home
  NULL_SCANCODE, //up
  NULL_SCANCODE, //pageup
  NULL_SCANCODE, //minus numpad 
  NULL_SCANCODE, //left
  NULL_SCANCODE, //5 numpad
  NULL_SCANCODE, //right
  NULL_SCANCODE, //plus numpad
  NULL_SCANCODE, //more numpad
  NULL_SCANCODE, //more numpad
  NULL_SCANCODE, //more numpad
  NULL_SCANCODE, //more numpad
  NULL_SCANCODE, //insert
  NULL_SCANCODE, //delete
  NULL_SCANCODE, 
  NULL_SCANCODE,
  NULL_SCANCODE, //f11
  NULL_SCANCODE, //f12
  NULL_SCANCODE, 
};

// Scancode conversion table to take in scancode and convert it to proper outputted character value with shift
static const uint8_t shiftkbrdscancode[SCANCODE_LENGTH] = { 
  BLANK_SCANCODE, 
  ESCAPE_SCANCODE, 
  EXCLAMATION_SCANCODE, 
  AT_SCANCODE, 
  HASH_SCANCODE, 
  DOLLAR_SCANCODE, 
  PERCENT_SCANCODE, 
  CARET_SCANCODE,
  AMPERSAND_SCANCODE, 
  STAR_SCANCODE, 
  OPENPARENTHESIS_SCANCODE, 
  CLOSEPARENTHESIS_SCANCODE, 
  UNDERSCORE_SCANCODE, 
  PLUS_SCANCODE, 
  BACKSPACE_SCANCODE, 
  TAB_SCANCODE,
  Q_CAPS_SCANCODE,
  W_CAPS_SCANCODE,
  E_CAPS_SCANCODE,
  R_CAPS_SCANCODE,
  T_CAPS_SCANCODE,
  Y_CAPS_SCANCODE,
  U_CAPS_SCANCODE,
  I_CAPS_SCANCODE,
  O_CAPS_SCANCODE,
  P_CAPS_SCANCODE,
  OPENCURLB_SCANCODE,
  CLOSECURLB_SCANCODE,
  ENTER_SCANCODE,
  NULL_SCANCODE,
  A_CAPS_SCANCODE,
  S_CAPS_SCANCODE,
  D_CAPS_SCANCODE,
  F_CAPS_SCANCODE,
  G_CAPS_SCANCODE,
  H_CAPS_SCANCODE,
  J_CAPS_SCANCODE,
  K_CAPS_SCANCODE,
  L_CAPS_SCANCODE,
  COLON_SCANCODE,
  DOUBLEQUOTE_SCANCODE,
  TILDE_SCANCODE,
  NULL_SCANCODE,
  LINE_SCANCODE,
  Z_CAPS_SCANCODE,
  X_CAPS_SCANCODE,
  C_CAPS_SCANCODE,
  V_CAPS_SCANCODE,
  B_CAPS_SCANCODE,
  N_CAPS_SCANCODE,
  M_CAPS_SCANCODE,
  LESS_SCANCODE,
  GREATER_SCANCODE,
  QUESTION_SCANCODE,
  NULL_SCANCODE, 
  STAR_SCANCODE,
  NULL_SCANCODE,  // left alt
  SPACE_SCANCODE,
  NULL_SCANCODE, //f1
  NULL_SCANCODE, //f2
  NULL_SCANCODE, //f3
  NULL_SCANCODE, //f4
  NULL_SCANCODE, //f5
  NULL_SCANCODE, //f6
  NULL_SCANCODE, //f7
  NULL_SCANCODE, //f8
  NULL_SCANCODE, //f9
  NULL_SCANCODE, //f9
  NULL_SCANCODE, //f10
  NULL_SCANCODE, //numlock
  NULL_SCANCODE, //scroll lock
  NULL_SCANCODE, //home
  NULL_SCANCODE, //up
  NULL_SCANCODE, //pageup
  NULL_SCANCODE, //minus numpad 
  NULL_SCANCODE, //left
  NULL_SCANCODE, //5 numpad
  NULL_SCANCODE, //right
  NULL_SCANCODE, //plus numpad
  NULL_SCANCODE, //more numpad
  NULL_SCANCODE, //more numpad
  NULL_SCANCODE, //more numpad
  NULL_SCANCODE, //more numpad
  NULL_SCANCODE, //insert
  NULL_SCANCODE, //delete
  NULL_SCANCODE, 
  NULL_SCANCODE,
  NULL_SCANCODE, //f11
  NULL_SCANCODE, //f12
  NULL_SCANCODE, 
};

// Scancode conversion table to take in scancode and convert it to proper outputted character value with shift and caps lock
static const uint8_t capsshiftkbrdscancode[SCANCODE_LENGTH] = { 
  BLANK_SCANCODE, 
  ESCAPE_SCANCODE, 
  EXCLAMATION_SCANCODE, 
  AT_SCANCODE, 
  HASH_SCANCODE, 
  DOLLAR_SCANCODE, 
  PERCENT_SCANCODE, 
  CARET_SCANCODE,
  AMPERSAND_SCANCODE, 
  STAR_SCANCODE, 
  OPENPARENTHESIS_SCANCODE, 
  CLOSEPARENTHESIS_SCANCODE, 
  UNDERSCORE_SCANCODE, 
  PLUS_SCANCODE, 
  BACKSPACE_SCANCODE, 
  TAB_SCANCODE,
  Q_SCANCODE,
  W_SCANCODE,
  E_SCANCODE,
  R_SCANCODE,
  T_SCANCODE,
  Y_SCANCODE,
  U_SCANCODE,
  I_SCANCODE,
  O_SCANCODE,
  P_SCANCODE,
  OPENCURLB_SCANCODE,
  CLOSECURLB_SCANCODE,
  ENTER_SCANCODE,
  NULL_SCANCODE,
  A_SCANCODE,
  S_SCANCODE,
  D_SCANCODE,
  F_SCANCODE,
  G_SCANCODE,
  H_SCANCODE,
  J_SCANCODE,
  K_SCANCODE,
  L_SCANCODE,
  COLON_SCANCODE,
  DOUBLEQUOTE_SCANCODE,
  TILDE_SCANCODE,
  NULL_SCANCODE,
  LINE_SCANCODE,
  Z_SCANCODE,
  X_SCANCODE,
  C_SCANCODE,
  V_SCANCODE,
  B_SCANCODE,
  N_SCANCODE,
  M_SCANCODE,
  LESS_SCANCODE,
  GREATER_SCANCODE,
  QUESTION_SCANCODE,
  NULL_SCANCODE, 
  STAR_SCANCODE,
  NULL_SCANCODE,  // left alt
  SPACE_SCANCODE,
  NULL_SCANCODE, //f1
  NULL_SCANCODE, //f2
  NULL_SCANCODE, //f3
  NULL_SCANCODE, //f4
  NULL_SCANCODE, //f5
  NULL_SCANCODE, //f6
  NULL_SCANCODE, //f7
  NULL_SCANCODE, //f8
  NULL_SCANCODE, //f9
  NULL_SCANCODE, //f9
  NULL_SCANCODE, //f10
  NULL_SCANCODE, //numlock
  NULL_SCANCODE, //scroll lock
  NULL_SCANCODE, //home
  NULL_SCANCODE, //up
  NULL_SCANCODE, //pageup
  NULL_SCANCODE, //minus numpad 
  NULL_SCANCODE, //left
  NULL_SCANCODE, //5 numpad
  NULL_SCANCODE, //right
  NULL_SCANCODE, //plus numpad
  NULL_SCANCODE, //more numpad
  NULL_SCANCODE, //more numpad
  NULL_SCANCODE, //more numpad
  NULL_SCANCODE, //more numpad
  NULL_SCANCODE, //insert
  NULL_SCANCODE, //delete
  NULL_SCANCODE, 
  NULL_SCANCODE,
  NULL_SCANCODE, //f11
  NULL_SCANCODE, //f12
  NULL_SCANCODE, 
};


int32_t terminal_open(const uint8_t* filename);

int32_t terminal_close(int32_t fd);

int32_t terminal_read(int32_t fd, uint8_t* buf, int32_t nbytes);

int32_t terminal_write(int32_t fd, const uint8_t* buf, int32_t nbytes);


void kbrd_init();

void kbrd_int();



#endif
