#ifndef RAW_MODE
#define RAW_MODE

/*** includes ***/
#include <stdio.h>      // printf()
#include <ctype.h>      // iscntrl()
#include <stdlib.h>     // atexit()
#include <unistd.h>     // read(), write(), STDIN_FILENO
#include <termios.h>    // (linux only) struct termios, tcgetattr(), tcsetattr(), TCSAFLUSH, ECHO, ICANON, ISIG, IXON
#include <errno.h>

/*** definition ***/
void enableRawMode();
void disableRawMode();
void die(const char* s);

#endif