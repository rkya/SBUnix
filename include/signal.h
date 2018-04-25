#ifndef _SIGNAL_H
#define _SIGNAL_H

#include <sys/defs.h>
#include <stdio.h>
#include <sys/process.h>

#define SIGKILL 9

int kill(pid_t pid, int sig);
void shutdownOS();

// OPTIONAL: implement for ``signals and pipes (+10 pts)''
typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);

#endif
