#include <sys/timer.h>
#include <sys/kprintf.h>

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
//    adjustment = 0;
    for(int i = 0; i < MAX_PROCESSES; i++) {
      if(active_queue[i] != NULL && active_queue[i]->state == SLEEPING) {
        pcb *temp = p_get_current_process();
        int id;
        id = temp == NULL ? -1 : temp->pid;
        kprintf("Current process = %d, i = %d, remaining sleep time = %d",
        id, i, active_queue[i]->remaining_sleep_time);
        active_queue[i]->remaining_sleep_time--;
        if(active_queue[i]->remaining_sleep_time <= 0) {
          active_queue[i]->state = RUNNING;
        }
      }
    }
    adjustment = 0;
    //Since this is a preemptive OS, the process switches to a new one after every 1 second
    //The scheduler decides which process should continue execution next
    schedule();
  }

  /*if(adjustment >= 1000) {
    adjustment = 0;
    schedule();
  }*/

  pcb *current_process = p_get_current_process();
  if(current_process != NULL && current_process->pid == 0) {
    //t_timer_interrupt_action();
    //kprintf("Current pid = %d\n", current_process->pid);
    //yield();
  }
}