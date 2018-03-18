#include <sys/keyboard.h>
#include <sys/kprintf.h>

static int shift = 0;
static int ctrl = 0;

uint8_t hex_to_char(uint8_t);

uint8_t hex_to_char(uint8_t key) {

  if(key == KB_L_SHIFT || key == KB_R_SHIFT || key == KB_CAPS) {
    shift = 1;
    return 0;
  }

  if(key == KB_L_CONTROL) {
    ctrl = 1;
    t_write_to_screen("CTRL");
    return 0;
  }

  if(ctrl == 1) {
    ctrl = 0;
    if(key == KB_C) {
      t_write_to_screen("^C");
    } else if(key == KB_Z) {
      t_write_to_screen("^Z");
    } else if(key == KB_D) {
      t_write_to_screen("^D");
    }
    return 0;
  }

  //case where shift key is pressed
  if(shift == 1) {
    shift = 0;
    if(key >= KB_1 && key <= KB_EQUAL_PLUS) {
      return "!@#$%^&*()_+"[key - KB_1];
    }

    if(key >= KB_Q && key <= KB_BRACES_CLOSE) {
      return "QWERTYUIOP{}"[key - KB_Q];
    }

    if(key >= KB_A && key <= KB_INVERTED_COMMA) {
      return "ASDFGHJKL:\""[key - KB_A];
    }

    if(key >= KB_Z && key <= KB_F_SLASH_QUESTION) {
      return "ZXCVBNM<>?"[key - KB_Z];
    }

    if(key == KB_TILDE) {
      return '~';
    }

    if(key == KB_B_SLASH) {
      return '|';
    }

    return 0;
  }

  if(key >= KB_1 && key <= KB_EQUAL_PLUS) {
    return "1234567890-="[key - KB_1];
  }

  if(key >= KB_Q && key <= KB_BRACES_CLOSE) {
    return "qwertyuiop[]"[key - KB_Q];
  }

  if(key >= KB_A && key <= KB_INVERTED_COMMA) {
    return "asdfghjkl;'"[key - KB_A];
  }

  if(key >= KB_Z && key <= KB_F_SLASH_QUESTION) {
    return "zxcvbnm,./"[key - KB_Z];
  }

  if(key >= KB_KEYPAD_7 && key <= KB_KEYPAD_DOT) {
    return "789-456+1230."[key - KB_KEYPAD_7];
  }

  if(key == KB_TILDE) {
    return '`';
  }

  if(key == KB_B_SLASH) {
    return '\\';
  }

  if(key == KB_SPACE) {
    return ' ';
  }

  if(key == KB_ENTER) {
    t_add_to_buffer('\n');
  }

  if(key == KB_BACKSPACE) {
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
