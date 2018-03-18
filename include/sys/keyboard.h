#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <sys/defs.h>
#include <sys/isr.h>
#include <sys/io.h>

/*keyboard scan codes referred from https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html*/
enum scan_codes {
  KB_ERR = 0x00,
  KB_ESC = 0x01,

  KB_1 = 0x02,
  KB_2 = 0x03,
  KB_3 = 0x04,
  KB_4 = 0x05,
  KB_5 = 0x06,
  KB_6 = 0x07,
  KB_7 = 0x08,
  KB_8 = 0x09,
  KB_9 = 0x0a,
  KB_0 = 0x0b,

  KB_MINUS_UNDERSCORE = 0x0c,
  KB_EQUAL_PLUS = 0x0d,
  KB_BACKSPACE = 0x0e,

  KB_TAB = 0x0f,
  KB_Q = 0x10,
  KB_W = 0x11,
  KB_E = 0x12,
  KB_R = 0x13,
  KB_T = 0x14,
  KB_Y = 0x15,
  KB_U = 0x16,
  KB_I = 0x17,
  KB_O = 0x18,
  KB_P = 0x19,
  KB_BRACES_OPEN = 0x1a,
  KB_BRACES_CLOSE = 0x1b,

  KB_ENTER = 0x1c,

  KB_L_CONTROL = 0x1d,

  KB_A = 0x1e,
  KB_S = 0x1f,
  KB_D = 0x20,
  KB_F = 0x21,
  KB_G = 0x22,
  KB_H = 0x23,
  KB_J = 0x24,
  KB_K = 0x25,
  KB_L = 0x26,
  KB_SEMICOLON_COMMA = 0x27,
  KB_INVERTED_COMMA = 0x28,

  KB_TILDE = 0x29,

  KB_L_SHIFT = 0x2a,

  KB_B_SLASH = 0x2b,

  KB_Z = 0x2c,
  KB_X = 0x2d,
  KB_C = 0x2e,
  KB_V = 0x2f,
  KB_B = 0x30,
  KB_N = 0x31,
  KB_M = 0x32,
  KB_COMMA_ANGLE_OPEN = 0x33,
  KB_DOT_ANGLE_CLOSE = 0x34,
  KB_F_SLASH_QUESTION = 0x35,
  KB_R_SHIFT = 0x36,

  KB_STAR = 0x37,

  KB_L_ALT = 0x38,
  KB_SPACE = 0x39,

  KB_CAPS = 0x3a,

  KB_F1 = 0x3b,
  KB_F2 = 0x3c,
  KB_F3 = 0x3d,
  KB_F4 = 0x3e,
  KB_F5 = 0x3f,
  KB_F6 = 0x40,
  KB_F7 = 0x41,
  KB_F8 = 0x42,
  KB_F9 = 0x43,
  KB_F10 = 0x44,

  KB_NUM_LOCK = 0x45,

  KB_SCROLL_LOCK = 0x46,

  KB_KEYPAD_7 = 0x47,
  KB_KEYPAD_8 = 0x48,
  KB_KEYPAD_9 = 0x49,

  KB_KEYPAD_MINUS = 0x4a,

  KB_KEYPAD_4 = 0x4b,
  KB_KEYPAD_5 = 0x4c,
  KB_KEYPAD_6 = 0x4d,
  KB_KEYPAD_PLUS = 0x4e,

  KB_KEYPAD_1 = 0x4f,
  KB_KEYPAD_2 = 0x50,
  KB_KEYPAD_3 = 0x51,

  KB_KEYPAD_0 = 0x52,
  KB_KEYPAD_DOT = 0x53,

  KB_F11 = 0x57,
  KB_F12 = 0x58
};

void keyboard_interrupt();

#endif
