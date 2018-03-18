#include <sys/kprintf.h>
#include <sys/io.h>
#include <sys/util.h>
//#include <sys/process.h>

#define kernbase 0xffffffff80000000

//extern void yield();
enum dataTypes {character, integer, hex, string, pointer};
//#define BUFFER 256
volatile char *videoCardEnd = (volatile char *)0xB8FA0;
volatile char *videoCardStart = (volatile char *)0xB8000;
volatile char *videoCardPosition = (volatile char *)0xB8000;
int scrollForNextCall = 0;
int buffer_head = 0;
int buffer_tail = 0;
int buffer_size = 0;
int WAIT_FOR_USER_INPUT = 0;
registers regs;

void scroll();
void print_new_line(volatile char ** );
//char* return_to_foreground_process();

/***
 * This function is to be called only after setting the cr3 so that the video card
 * is now accessed by it's virtual address.
 */
void t_set_new_video_card_addresses() {
  videoCardEnd = (volatile char *)0xFFFFFFFF800B8FA0;
  videoCardStart = (volatile char *)0xFFFFFFFF800B8000;
  videoCardPosition = kernbase + videoCardPosition;
}

/***
 * Print a pointer to the screen.
 * @param video location from where pointer is to be printed
 * @param colour of the text
 * @param num pointer value
 * @return offset from where actual value is to be printed
 */
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

/***
 * Print hex value to the screen.
 * @param video location from where hex value is to be printed
 * @param colour of the text
 * @param num hex value
 * @return offset from where actual value is to be printed
 */
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

/***
 * Print integer value to the screen.
 * @param video location from where integer is to be printed
 * @param colour of the text
 * @param num integer value
 * @return offset from where actual value is to be prited
 */
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

/***
 * Moves the cursor to the new line on the terminal.
 * @param video the current position of the cursor on the screen
 */
void print_new_line(volatile char ** video) {
  if((videoCardPosition - videoCardStart) % 160) {
    (*video) += 160 - (((*video) - videoCardStart) % 160);
  } else {
    (*video) += 160 - ((*video) - videoCardPosition) % 160;
  }
}

/***
 * Write the text to the screen.
 * @param fmt text to be printed
 * @param ... optional parameters
 */
void t_write_to_screen(const char *fmt, ...) {
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
          if(ch == '\n') {
            print_new_line(&video);
          } else {
            *video++ = ch;
            *video++ = colour;
          }
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
      /*if((videoCardPosition - videoCardStart) % 160) {
        video += 160 - ((video - videoCardStart) % 160);
      } else {
        video += 160 - (video - videoCardPosition) % 160;
      }*/
      print_new_line(&video);
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

/***
 * Initialize the complete screen to blank and move the cursor at the start of the screen.
 */
void t_init_screen() {
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

/***
 * Scroll one line.
 */
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

char t_read_char_from_screen() {

//  u_save_state(&regs);
  while (!WAIT_FOR_USER_INPUT);

  char ch = buffer[buffer_head];
  buffer_head = (buffer_head + 1) % MAX_BUFFER_SIZE;
  buffer_size--;

  if(ch == '\0') {
    WAIT_FOR_USER_INPUT--;
  }

  return ch;
}

char *t_read_line_from_screen() {
  char *arr = (char *) kmalloc(sizeof(char) * (buffer_size + 1));
  int i = 0;
  for(int j = buffer_head;
      buffer_size > 0 && buffer[j] != '\0';
      buffer_size--, j = (j + 1) % MAX_BUFFER_SIZE, i++) {
    arr[i] = buffer[j];
  }
  arr[i] = '\0';

  buffer_head = 0;
  buffer_tail = 0;
  buffer_size = 0;
  WAIT_FOR_USER_INPUT = 1;

  return arr;
}

/*char *return_to_foreground_process() {

  //find foreground process
  return buffer;
}*/

/***
 * Add the character to the terminal buffer.
 * This is to be called by the keyboard interrupt when it receives a charater.
 * @param ch to be added to the terminal buffer
 */
void t_add_to_buffer(char ch) {
  if(ch == '\n') {
    if(buffer_size >= MAX_BUFFER_SIZE) {
      if(buffer_tail == 0) {
        buffer[MAX_BUFFER_SIZE - 1] = '\0';
      } else {
        buffer[buffer_tail - 1] = '\0';
      }
    } else {
      buffer[buffer_tail % MAX_BUFFER_SIZE] = '\0';
      buffer_tail = (buffer_tail + 1) % MAX_BUFFER_SIZE;
      buffer_size++;
    }
    WAIT_FOR_USER_INPUT++;
    //u_revive_state(&regs);
  } else if(buffer_size < MAX_BUFFER_SIZE) {
    buffer[buffer_tail % MAX_BUFFER_SIZE] = ch;
    buffer_tail = (buffer_tail + 1) % MAX_BUFFER_SIZE;
    buffer_size++;
  }

  t_write_to_screen("%c", ch);
}

/***
 * Print a backspace on the screen.
 * It includes erasing the previous printed character on the screen and
 * moving the cursor backwards by one step.
 */
void t_backspace() {
  //erase previous character from buffer
  if(buffer_tail > 0) {
    buffer_tail--;
  }

  //set the previous character as blank on screen
  videoCardPosition -= 2;
  if(videoCardPosition < videoCardStart) {
    videoCardPosition = videoCardStart;
  }
  *videoCardPosition = ' ';

  //set the new cursor position
  outb(0x3D4, 0x0F);
  outb(0x3D5, ((videoCardPosition - videoCardStart) / 2) & 0xFF);
  outb(0x3D4, 0x0E);
  outb(0x3D5, (((videoCardPosition - videoCardStart) / 2) >> 8) & 0xFF);
}