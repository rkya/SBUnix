#include <sys/kprintf.h>

//References: https://wiki.osdev.org/Printing_To_Screen

//#include <sys/io.h>

/*
#define kernbase 0xffffffff80000000

enum dataTypes {character, integer, hex, string, pointer};
//#define BUFFER 256
volatile char *videoCardEnd = (volatile char *)0xB8FA0;
volatile char *videoCardStart = (volatile char *)0xB8000;
volatile char *videoCardPosition = (volatile char *)0xB8000;
int scrollForNextCall = 0;

void setNewVideoCardAddresses(){
  videoCardEnd = (volatile char *)0xFFFFFFFF800B8FA0;
  videoCardStart = (volatile char *)0xFFFFFFFF800B8000;
  videoCardPosition = kernbase + videoCardPosition;
}

int printPointer(volatile char *video, int colour, uint64_t num) {
  if(num == 0) {
    return 0;
  }
  int counter = printPointer(video, colour, num / 16);
  int digit = num % 16;
  if(digit < 0) {
    digit *= -1;
  }
  if(digit >= 10) {
    digit += 87;
  } else {
    digit += 48;
  }
  *(video + counter) = digit;
  *(video + counter + 1) = colour;
  return 2 + counter;
}

int printHex(volatile char *video, int colour, int num) {
  if(num == 0) {
    return 0;
  }
  int counter = printHex(video, colour, num / 16);
  int digit = num % 16;
  if(digit >= 10) {
    digit += 87;
  } else {
    digit += 48;
  }
  *(video + counter) = digit;
  *(video + counter + 1) = colour;
  return 2 + counter;
}

int printInt(volatile char *video, int colour, int num) {
  if(num == 0) {
    *(video) = 48;
    *(video + 1) = colour;
    return 0;
  }
  if(num < 0) {
    *(video) = 45;
    *(video + 1) = colour;
    num = num * -1;
    *(video)++;
    *(video)++;
  }

  int counter = printInt(video, colour, num / 10);
  *(video + counter) = num % 10 + 48;
  *(video + counter + 1) = colour;
  return 2 + counter;
}

void kprintf(const char *fmt, ...) {
  va_list parameters;
  va_start(parameters, fmt);

  int colour = 0x00;
  int num = 0;
  uint64_t pointer = 0;
  volatile char *video = videoCardPosition;
  while(*fmt != 0) {
    if(scrollForNextCall) {
      scroll();
      video -= 160;
      videoCardPosition -= 160;
      scrollForNextCall = 0;
    }
    if(*fmt == '%') {
      fmt++;
      switch(*fmt) {
        case 's':;
          char *str = va_arg(parameters, char *);
          while(*str != 0) {
            *video++ = *str++;
            *video++ = colour;
          }
          break;
        case 'c':;
          char ch = va_arg(parameters, int);
          *video++ = ch;
          *video++ = colour;
          break;
        case 'd':;
          num = va_arg(parameters, int);
          video += printInt(video, colour, num);
          break;
        case 'x':;
          num = va_arg(parameters, int);
          video += printHex(video, colour, num);
          break;
        case 'p':;
          *video++ = '0';
          *video++ = colour;
          *video++ = 'x';
          *video++ = colour;
          pointer = (uint64_t)va_arg(parameters, void *);
          video += printPointer(video, colour, pointer);
          break;
      }
      fmt++;
    } else if(*fmt == '\n') {
      if((videoCardPosition - videoCardStart) % 160) {
        video += 160 - ((video - videoCardStart) % 160);
      } else {
        video += 160 - (video - videoCardPosition) % 160;
      }
      fmt++;
    } else {
      *video++ = *fmt++;
      *video++ = colour;
    }
    if(video >= videoCardEnd) {
      scrollForNextCall = 1;
    }
  }

  videoCardPosition = video;
  if(videoCardPosition >= videoCardEnd) {
    scrollForNextCall = 1;
  }

  //set cursor position
  outb(0x3D4, 0x0F);
  outb(0x3D5, ((videoCardPosition - videoCardStart) / 2) & 0xFF);
  outb(0x3D4, 0x0E);
  outb(0x3D5, (((videoCardPosition - videoCardStart) / 2) >> 8) & 0xFF);

  va_end(parameters);
}

void initScreen() {
  int i;
  register char *p = (char*) videoCardStart;
  for(i = 0; i < 4000; i++) {
    *p = ' ';
    p += 2;
  }

  //set cursor position
  outb(0x3D4, 0x0F);
  outb(0x3D5, 0 & 0xFF);
  outb(0x3D4, 0x0E);
  outb(0x3D5, (0 >> 8) & 0xFF);
}

void scroll() {
  int i, j;
  register char *p = (char *) videoCardStart;

  //for first 24 lines, copy contents from the next line
  for(i = 0; i < 24; i++) {
    for(j = 0; j < 160; j++) {
      *p = *(p + 160);
      p++;
    }
  }
  //for the last line, print blank spaces
  for(j = 0; j < 160 && p <= videoCardEnd; j++) {
    *p = ' ';
    p += 2;
  }
}
*/

#include <stdarg.h>

#define KPRINTF_BUFFER_SIZE 4096
char kprintf_buffer[KPRINTF_BUFFER_SIZE];
int kprintf_buffer_index;

/***
 * Print a pointer to the screen.
 * @param index location from where pointer is to be printed
 * @param colour of the text
 * @param num pointer value
 * @return offset from where actual value is to be printed
 */
int kprintf_printPointer(int index, uint64_t num) {
  if(num == 0) {
    return 0;
  }
  int counter = kprintf_printPointer(index, num / 16);
  int digit = num % 16;
  if(digit < 0) {
    digit *= -1;
  }
  if(digit >= 10) {
    digit += 87;
  } else {
    digit += 48;
  }
  kprintf_buffer[index + counter] = digit;
  return 1 + counter;
}

/***
 * Print hex value to the screen.
 * @param index location from where hex value is to be printed
 * @param colour of the text
 * @param num hex value
 * @return offset from where actual value is to be printed
 */
int kprintf_printHex(int index, int num) {
  if(num == 0) {
    return 0;
  }
  int counter = kprintf_printHex(index, num / 16);
  int digit = num % 16;
  if(digit >= 10) {
    digit += 87;
  } else {
    digit += 48;
  }
  kprintf_buffer[index + counter] = digit;
  return 1 + counter;
}

/***
 * Print integer value to the screen.
 * @param index location from where integer is to be printed
 * @param colour of the text
 * @param num integer value
 * @return offset from where actual value is to be prited
 */
int kprintf_printInt(int index, int num) {
  if(num == 0) {
    kprintf_buffer[index] = 48;
    return 0;
  }
  if(num < 0) {
    kprintf_buffer[index] = 45;
    num = num * -1;
    index++;
  }

  int counter = kprintf_printInt(index, num / 10);
  kprintf_buffer[index + counter] = num % 10 + 48;
  return 1 + counter;
}


const char *kprintf_format_string(const char *fmt, ...) {
  va_list parameters;
  va_start(parameters, fmt);

  kprintf_buffer_index = 0;
  int num = 0;
  uint64_t pointer = 0;
  while(*fmt != 0 && kprintf_buffer_index < KPRINTF_BUFFER_SIZE - 1) {
    if(*fmt == '%') {
      fmt++;
      switch(*fmt) {
        case 's':;
          char *str = va_arg(parameters, char *);
          while(*str != 0 && *str != '\0') {
            kprintf_buffer[kprintf_buffer_index++] = *str;
            str++;
          }
          break;
        case 'c':;
          char ch = va_arg(parameters, int);
          kprintf_buffer[kprintf_buffer_index++] = ch;
          break;
        case 'd':;
          num = va_arg(parameters, int);
          if(num == 0) {
            kprintf_buffer[kprintf_buffer_index++] = '0';
          } else {
            kprintf_buffer_index += kprintf_printInt(kprintf_buffer_index, num);
          }
          break;
        case 'x':;
          num = va_arg(parameters, int);
          if(num == 0) {
            kprintf_buffer[kprintf_buffer_index++] = '0';
          } else {
            kprintf_buffer_index += kprintf_printHex(kprintf_buffer_index, num);
          }
          break;
        case 'p':;
          kprintf_buffer[kprintf_buffer_index++] = '0';
          kprintf_buffer[kprintf_buffer_index++] = 'x';
          pointer = (uint64_t)va_arg(parameters, void *);
          if(pointer == 0) {
            kprintf_buffer[kprintf_buffer_index++] = '0';
          } else {
            kprintf_buffer_index += kprintf_printPointer(kprintf_buffer_index, pointer);
          }
          break;
      }
      fmt++;
    } else {
      kprintf_buffer[kprintf_buffer_index++] = *fmt;
      fmt++;
    }
  }
  kprintf_buffer[kprintf_buffer_index] = '\0';

  va_end(parameters);

  if(kprintf_buffer_index >= KPRINTF_BUFFER_SIZE - 1 && *fmt != 0) {
    return fmt;
  }
  return NULL;
}

/***
 * Writes the string to the screen in the specified format.
 * @param fmt string to be printed to the screen
 * @param ... optional parameters
 */
void kprintf(const char *fmt, ...) {
  kprintf_buffer_index = 0;

  while (1) {
    kprintf_buffer[0] = '\0';
    const char *ptr = kprintf_format_string(fmt);

    t_write_to_screen(kprintf_buffer);

    if(ptr == NULL || *ptr == 0) {
      break;
    }
    fmt = ptr;
  }
}

void kprintf_formatted(const char *fmt) {
  t_write_to_screen(fmt);
}