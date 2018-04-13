#include <sys/timer.h>
#include <sys/kprintf.h>

void t_init_timer() {
  //divisor = 1193180 Hz / frequency (in Hz)
  uint32_t divisor = 1193180 / 100;
  outb(0x43, 0x36);

  uint8_t l = (uint16_t) (divisor & 0xFF);
  uint8_t h = (uint16_t) (divisor >> 8);

  outb(0x40, l);
  outb(0x40, h);
}