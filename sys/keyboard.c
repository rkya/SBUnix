#include <sys/keyboard.h>
#include <sys/kprintf.h>

static int shift = 0;
static int ctrl = 0;

uint8_t hex_to_char(uint8_t);

uint8_t hex_to_char(uint8_t key) {

  if(key == KEYBOARD_L_SHIFT || key == KEYBOARD_R_SHIFT || key == KEYBOARD_CAPS) {
    shift = 1;
    return 0;
  }

  if(key == KEYBOARD_L_CONTROL) {
    ctrl = 1;
    t_write_to_screen("CTRL");
    return 0;
  }

  if(ctrl == 1) {
    ctrl = 0;
    if(key == KEYBOARD_C) {
      t_write_to_screen("^C");
    } else if(key == KEYBOARD_Z) {
      t_write_to_screen("^Z");
    } else if(key == KEYBOARD_D) {
      t_write_to_screen("^D");
    }
    return 0;
  }

  //case where shift key is pressed
  if(shift == 1) {
    shift = 0;
    if(key >= KEYBOARD_1 && key <= KEYBOARD_EQUAL_PLUS) {
      return "!@#$%^&*()_+"[key - KEYBOARD_1];
    }

    if(key >= KEYBOARD_Q && key <= KEYBOARD_BRACES_CLOSE) {
      return "QWERTYUIOP{}"[key - KEYBOARD_Q];
    }

    if(key >= KEYBOARD_A && key <= KEYBOARD_INVERTED_COMMA) {
      return "ASDFGHJKL:\""[key - KEYBOARD_A];
    }

    if(key >= KEYBOARD_Z && key <= KEYBOARD_F_SLASH_QUESTION) {
      return "ZXCVBNM<>?"[key - KEYBOARD_Z];
    }

    if(key == KEYBOARD_TILDE) {
      return '~';
    }

    if(key == KEYBOARD_B_SLASH) {
      return '|';
    }

    return 0;
  }

  if(key >= KEYBOARD_1 && key <= KEYBOARD_EQUAL_PLUS) {
    return "1234567890-="[key - KEYBOARD_1];
  }

  if(key >= KEYBOARD_Q && key <= KEYBOARD_BRACES_CLOSE) {
    return "qwertyuiop[]"[key - KEYBOARD_Q];
  }

  if(key >= KEYBOARD_A && key <= KEYBOARD_INVERTED_COMMA) {
    return "asdfghjkl;'"[key - KEYBOARD_A];
  }

  if(key >= KEYBOARD_Z && key <= KEYBOARD_F_SLASH_QUESTION) {
    return "zxcvbnm,./"[key - KEYBOARD_Z];
  }

  if(key >= KEYBOARD_KEYPAD_7 && key <= KEYBOARD_KEYPAD_DOT) {
    return "789-456+1230."[key - KEYBOARD_KEYPAD_7];
  }

  if(key == KEYBOARD_TILDE) {
    return '`';
  }

  if(key == KEYBOARD_B_SLASH) {
    return '\\';
  }

  if(key == KEYBOARD_SPACE) {
    return ' ';
  }

  if(key == KEYBOARD_ENTER) {
    t_add_to_buffer('\n');
  }

  if(key == KEYBOARD_BACKSPACE) {
    t_backspace();
  }

  return 0;
}

void keyboard_interrupt() {
  uint8_t key;
  uint16_t port = 0x60;

  __asm__ __volatile__(
      "inb %1, %0;"
    : "=a" (key)
    : "d" (port)
  );

  port = 0x20;
  uint8_t data = 0x20;

  __asm__ __volatile__(
      "outb %0, %1;"
    :
    : "a" (data), "d" (port)
  );

  uint8_t character = hex_to_char(key);
  if(character != 0) {
    t_add_to_buffer(character);
  } else {
    //kprintf("wrong input! %d \n%x\n", character, key);
  }
}
