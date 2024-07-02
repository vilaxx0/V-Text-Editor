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

    int rowOffset;              // Row that the user is currently scrolled to (if 0 its top of the file)
    int colOffset;
    
    int nrOfRows;               // Number of rows
    struct EditorRow *row;

    // Original terminal attributes
    struct termios originalTermios;
};

extern struct EditorConfig editor;

void die(const char* s);

#endif