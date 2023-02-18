#include "editor.h"

// Original terminal attributes
struct termios original_termios;

/*
    1. Removes "ECHO" feature which causes each key press to be printed to the terminal.
    This is useful in canonical mode, but really gets in the way when we are trying to carefully render a user interface in raw mode.

    2. Removes "ICANON" feature which causes canonical mode that reads line-by-line on ENTER
    This will allow to read input byte-by-byte and without the need of pressing ENTER

    3. Removes "ISIG" features which when pressed:
    "Ctrl-C" sends a SIGINT signal to the current process which causes it to terminate
    "Ctrl-Z" sends a SIGTSTP signal to the current process which causes it to suspend
*/ 
void enableRawMode() {
    tcgetattr(STDOUT_FILENO, &original_termios);
    atexit(disableRawMode);
    
    /* 
        ECHO, ICANON, ISIG, IXON    terminal attribute defined as in binary as a bitflag (e.g "00000000 00000000 00000000 00001000")
        ~                           (bitwise-NOT operator) flips all bits
        &                           (bitwise-AND operator) returns bits set to 1 if both bits corresponding position are 1
        |                           (bitwise-OR operator) returns bits set to 1 if at least one bits in corresponding position are 1

        c_lflag                     "local flags"
        c_iflag                     "input flags"
        c_oflag                     "output flags"
        c_cflag                     "control flags"
    */
    struct termios raw_termios = original_termios;
    raw_termios.c_iflag &= ~(IXON);
    raw_termios.c_lflag &= ~(ECHO | ICANON | ISIG);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_termios);
}

// Disabling RAW mode by setting back the original terminal attributes
void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
}