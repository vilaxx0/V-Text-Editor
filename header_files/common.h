#ifndef COMMON_H
#define COMMON_H

#define CTRL_KEY(k) ((k) & 0x1f)

#include <termios.h>    // struct termios, tcgetattr(), tcsetattr(), TCSAFLUSH, ECHO, ICANON, ISIG, IXON

typedef struct EditorRow {
    int size;
    char *chars;
} EditorRow; 

struct EditorConfig {
    int cursorX;
    int cursorY;
    int screenCols;
    int screenRows;
    
    int nrOfRows;
    struct EditorRow row;

    // Original terminal attributes
    struct termios original_termios;
};

extern struct EditorConfig editor;

void die(const char* s);

#endif