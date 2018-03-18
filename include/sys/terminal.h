#ifndef _TERMINAL_H
#define _TERMINAL_H

#include <stdarg.h>
#include <stdlib.h>
#include <sys/paging.h>

#define MAX_BUFFER_SIZE 256

char buffer[MAX_BUFFER_SIZE];

void t_write_to_screen(const char *fmt, ...);
char t_read_char_from_screen();
char *t_read_line_from_screen();
void t_add_to_buffer(char);
void t_init_screen();
void t_backspace();
void t_set_new_video_card_addresses();

#endif
