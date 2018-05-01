#include <stdio.h>
#include <string.h>
#include <sys/terminal.h>
#include <sys/syscall.h>
#include <stdarg.h>

//extern int return_value;
#define KPRINTF_BUFFER_SIZE 4096
char printf_buffer[KPRINTF_BUFFER_SIZE];
int printf_buffer_index;

/***
 * Print a pointer to the screen.
 * @param index location from where pointer is to be printed
 * @param colour of the text
 * @param num pointer value
 * @return offset from where actual value is to be printed
 */
int printf_printPointer(int index, uint64_t num) {
  if(num == 0) {
    return 0;
  }
  int counter = printf_printPointer(index, num / 16);
  int digit = num % 16;
  if(digit < 0) {
    digit *= -1;
  }
  if(digit >= 10) {
    digit += 87;
  } else {
    digit += 48;
  }
  printf_buffer[index + counter] = digit;
  return 1 + counter;
}

/***
 * Print hex value to the screen.
 * @param index location from where hex value is to be printed
 * @param colour of the text
 * @param num hex value
 * @return offset from where actual value is to be printed
 */
int printf_printHex(int index, int num) {
  if(num == 0) {
    return 0;
  }
  int counter = printf_printHex(index, num / 16);
  int digit = num % 16;
  if(digit >= 10) {
    digit += 87;
  } else {
    digit += 48;
  }
  printf_buffer[index + counter] = digit;
  return 1 + counter;
}

/***
 * Print integer value to the screen.
 * @param index location from where integer is to be printed
 * @param colour of the text
 * @param num integer value
 * @return offset from where actual value is to be prited
 */
int printf_printInt(int index, int num) {
  if(num == 0) {
    printf_buffer[index] = 48;
    return 0;
  }
  if(num < 0) {
    printf_buffer[index] = 45;
    num = num * -1;
    index++;
  }

  int counter = printf_printInt(index, num / 10);
  printf_buffer[index + counter] = num % 10 + 48;
  return 1 + counter;
}


/*const char *format_string(const char *fmt, ...) {
  va_list parameters;
  va_start(parameters, fmt);

  printf_buffer_index = 0;
  int num = 0;
  uint64_t pointer = 0;
  while(*fmt != 0 && printf_buffer_index < KPRINTF_BUFFER_SIZE - 1) {
    if(*fmt == '%') {
      fmt++;
      switch(*fmt) {
        case 's':;
          char *str = va_arg(parameters, char *);
          while(*str != 0 && *str != '\0') {
            printf_buffer[printf_buffer_index++] = *str;
            str++;
          }
          break;
        case 'c':;
          char ch = va_arg(parameters, int);
          printf_buffer[printf_buffer_index++] = ch;
          break;
        case 'd':;
          num = va_arg(parameters, int);
          if(num == 0) {
            printf_buffer[printf_buffer_index++] = '0';
          } else {
            printf_buffer_index += printf_printInt(printf_buffer_index, num);
          }
          break;
        case 'x':;
          num = va_arg(parameters, int);
          printf_buffer_index += printf_printHex(printf_buffer_index, num);
          break;
        case 'p':;
          printf_buffer[printf_buffer_index++] = '0';
          printf_buffer[printf_buffer_index++] = 'x';
          pointer = (uint64_t)va_arg(parameters, void *);
          printf_buffer_index += printf_printPointer(printf_buffer_index, pointer);
          break;
      }
      fmt++;
    } else {
      printf_buffer[printf_buffer_index++] = *fmt;
      fmt++;
    }
  }
  printf_buffer[printf_buffer_index] = '\0';

  va_end(parameters);

  if(printf_buffer_index >= KPRINTF_BUFFER_SIZE - 1 && *fmt != 0) {
    return fmt;
  }
  return NULL;
}*/


int printf(const char *format, ...) {

  /*char *arr = t_read_line_from_screen();
  char ds[3];
  strcpy(ds, arr);*/

  int multiplier = 0;
  printf_buffer_index = 0;
  while (1) {
    printf_buffer[0] = '\0';
    const char *ptr = NULL;

    va_list parameters;
    va_start(parameters, format);

    printf_buffer_index = 0;
    int num = 0;
    uint64_t pointer = 0;
    while(*format != 0 && printf_buffer_index < KPRINTF_BUFFER_SIZE - 1) {
      if(*format == '%') {
        format++;
        switch(*format) {
          case 's':;
            char *str = va_arg(parameters, char *);
            while(*str != 0 && *str != '\0') {
              printf_buffer[printf_buffer_index++] = *str;
              str++;
            }
            break;
          case 'c':;
            char ch = va_arg(parameters, int);
            printf_buffer[printf_buffer_index++] = ch;
            break;
          case 'd':;
            num = va_arg(parameters, int);
            if(num == 0) {
              printf_buffer[printf_buffer_index++] = '0';
            } else {
              printf_buffer_index += printf_printInt(printf_buffer_index, num);
            }
            break;
          case 'x':;
            num = va_arg(parameters, int);
            if(num == 0) {
              printf_buffer[printf_buffer_index++] = '0';
            } else {
              printf_buffer_index += printf_printHex(printf_buffer_index, num);
            }
            break;
          case 'p':;
            printf_buffer[printf_buffer_index++] = '0';
            printf_buffer[printf_buffer_index++] = 'x';
            pointer = (uint64_t)va_arg(parameters, void *);
            if(pointer == 0) {
              printf_buffer[printf_buffer_index++] = '0';
            } else {
              printf_buffer_index += printf_printPointer(printf_buffer_index, pointer);
            }
            break;
        }
        format++;
      } else {
        printf_buffer[printf_buffer_index++] = *format;
        format++;
      }
    }
    printf_buffer[printf_buffer_index] = '\0';

    va_end(parameters);

    if(printf_buffer_index >= KPRINTF_BUFFER_SIZE - 1 && *format != 0) {
      ptr = format;
    }

    int val;
    __asm__ __volatile__ (
    "int $0x80;"
    : "=a" (val)
    : "0"(SYSCALL_FORMATTED_KPRINTF), "D"(printf_buffer)
    : "cc", "rcx", "r11", "memory"
    );

    if(ptr == NULL || *ptr == 0) {
      break;
    }
    format = ptr;
    multiplier++;
  }

  return_value = multiplier * KPRINTF_BUFFER_SIZE + printf_buffer_index;
  return (int)return_value;
}
