#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <sys/defs.h>
#include <sys/isr.h>
#include <sys/io.h>

/*keyboard scan codes referred from https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html*/

#define KEYBOARD_ERR 0x00
#define KEYBOARD_ESC 0x01

#define KEYBOARD_1 0x02
#define KEYBOARD_2 0x03
#define KEYBOARD_3 0x04
#define KEYBOARD_4 0x05
#define KEYBOARD_5 0x06
#define KEYBOARD_6 0x07
#define KEYBOARD_7 0x08
#define KEYBOARD_8 0x09
#define KEYBOARD_9 0x0a
#define KEYBOARD_0 0x0b

#define KEYBOARD_MINUS_UNDERSCORE 0x0c
#define KEYBOARD_EQUAL_PLUS 0x0d
#define KEYBOARD_BACKSPACE 0x0e

#define KEYBOARD_TAB 0x0f
#define KEYBOARD_Q 0x10
#define KEYBOARD_W 0x11
#define KEYBOARD_E 0x12
#define KEYBOARD_R 0x13
#define KEYBOARD_T 0x14
#define KEYBOARD_Y 0x15
#define KEYBOARD_U 0x16
#define KEYBOARD_I 0x17
#define KEYBOARD_O 0x18
#define KEYBOARD_P 0x19
#define KEYBOARD_BRACES_OPEN 0x1a
#define KEYBOARD_BRACES_CLOSE 0x1b

#define KEYBOARD_ENTER 0x1c

#define KEYBOARD_L_CONTROL 0x1d

#define KEYBOARD_A 0x1e
#define KEYBOARD_S 0x1f
#define KEYBOARD_D 0x20
#define KEYBOARD_F 0x21
#define KEYBOARD_G 0x22
#define KEYBOARD_H 0x23
#define KEYBOARD_J 0x24
#define KEYBOARD_K 0x25
#define KEYBOARD_L 0x26
#define KEYBOARD_SEMICOLON_COMMA 0x27
#define KEYBOARD_INVERTED_COMMA 0x28

#define KEYBOARD_TILDE 0x29

#define KEYBOARD_L_SHIFT 0x2a

#define KEYBOARD_B_SLASH 0x2b

#define KEYBOARD_Z 0x2c
#define KEYBOARD_X 0x2d
#define KEYBOARD_C 0x2e
#define KEYBOARD_V 0x2f
#define KEYBOARD_B 0x30
#define KEYBOARD_N 0x31
#define KEYBOARD_M 0x32
#define KEYBOARD_COMMA_ANGLE_OPEN 0x33
#define KEYBOARD_DOT_ANGLE_CLOSE 0x34
#define KEYBOARD_F_SLASH_QUESTION 0x35
#define KEYBOARD_R_SHIFT 0x36

#define KEYBOARD_STAR 0x37

#define KEYBOARD_L_ALT 0x38
#define KEYBOARD_SPACE 0x39

#define KEYBOARD_CAPS 0x3a

#define KEYBOARD_F1 0x3b
#define KEYBOARD_F2 0x3c
#define KEYBOARD_F3 0x3d
#define KEYBOARD_F4 0x3e
#define KEYBOARD_F5 0x3f
#define KEYBOARD_F6 0x40
#define KEYBOARD_F7 0x41
#define KEYBOARD_F8 0x42
#define KEYBOARD_F9 0x43
#define KEYBOARD_F10 0x44

#define KEYBOARD_NUM_LOCK 0x45

#define KEYBOARD_SCROLL_LOCK 0x46

#define KEYBOARD_KEYPAD_7 0x47
#define KEYBOARD_KEYPAD_8 0x48
#define KEYBOARD_KEYPAD_9 0x49

#define KEYBOARD_KEYPAD_MINUS 0x4a

#define KEYBOARD_KEYPAD_4 0x4b
#define KEYBOARD_KEYPAD_5 0x4c
#define KEYBOARD_KEYPAD_6 0x4d
#define KEYBOARD_KEYPAD_PLUS 0x4e

#define KEYBOARD_KEYPAD_1 0x4f
#define KEYBOARD_KEYPAD_2 0x50
#define KEYBOARD_KEYPAD_3 0x51

#define KEYBOARD_KEYPAD_0 0x52
#define KEYBOARD_KEYPAD_DOT 0x53

#define KEYBOARD_F11 0x57
#define KEYBOARD_F12 0x58

void keyboard_interrupt();

#endif
