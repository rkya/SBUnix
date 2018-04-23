#ifndef TIMER_H
#define TIMER_H

#include <sys/defs.h>
#include <sys/io.h>
#include <sys/process.h>

void t_init_timer();
void t_timer_interrupt_action();

#endif