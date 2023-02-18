#ifndef VTD_H
#define VTD_H

#include <stdio.h>      // printf()
#include <ctype.h>      // iscntrl()
#include <stdlib.h>     // atexit()
#include <unistd.h>     // read(), write(), STDIN_FILENO
#include <termios.h>    // (linux only) struct termios, tcgetattr(), tcsetattr(), TCSAFLUSH, ECHO, ICANON, ISIG, IXON

void disableRawMode();
void enableRawMode();

#endif