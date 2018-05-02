#ifndef TIMER_H
#define TIMER_H

//References: http://www.jamesmolloy.co.uk/tutorial_html/5.-IRQs%20and%20the%20PIT.html

#include <sys/defs.h>
#include <sys/io.h>
#include <sys/process.h>

void t_init_timer();
void t_timer_interrupt_action();

#endif