#include <sys/timer.h>
#include <sys/kprintf.h>
#include "../include/sys/process.h"

//Reference: james molloy blog - http://www.jamesmolloy.co.uk/tutorial_html/5.-IRQs%20and%20the%20PIT.html
void t_init_timer() {
  //divisor = 1193180 Hz / frequency (in Hz)
  uint32_t divisor = 1193180 / 100;
  outb(0x43, 0x36);

  uint8_t l = (uint16_t) (divisor & 0xFF);
  uint8_t h = (uint16_t) (divisor >> 8);

  outb(0x40, l);
  outb(0x40, h);
}

static int adjustment = 0;
void t_timer_interrupt_action() {
  adjustment++;
  //adjustment to make the timer work for the interval of 1 second as sleep() takes an input in seconds.
  //the timer interrupt is received every 0.01 second as the frequency in the above function is 100Hz.
  //https://stackoverflow.com/questions/175882/whats-the-algorithm-behind-sleep
  if(adjustment == 100) {
    for(int i = 0; i < MAX_PROCESSES; i++) {
      if(active_queue[i] != NULL && active_queue[i]->state == SLEEPING) {
        active_queue[i]->remaining_sleep_time--;
        if(active_queue[i]->remaining_sleep_time <= 0) {
          active_queue[i]->state = RUNNING;
        }
      }
    }
    adjustment = 0;
  }
}