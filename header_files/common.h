#ifndef COMMON_H
#define COMMON_H

#define CTRL_KEY(k) ((k) & 0x1f)

#include <termios.h>    // struct termios, tcgetattr(), tcsetattr(), TCSAFLUSH, ECHO, ICANON, ISIG, IXON

void die(const char* s);
struct EditorConfig {
    int cursorX, cursorY;
    int screenRows;
    int screenCols;
    // Original terminal attributes
    struct termios original_termios;
};

extern struct EditorConfig editor;

#endif