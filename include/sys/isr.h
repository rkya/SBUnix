#ifndef _ISR_H
#define _ISR_H

#include <sys/defs.h>
#include <sys/idt.h>
#include <sys/io.h>
#include <sys/keyboard.h>
#include <sys/registers.h>

void fault_handler(registers );

#endif
