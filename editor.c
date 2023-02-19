#include "editor.h"

// Original terminal attributes
struct termios original_termios;

/*
    1. Removes "ECHO" feature which causes each key press to be printed to the terminal.
    *This is useful in canonical mode, but really gets in the way when we are trying to carefully render a user interface in raw mode.

    2. Removes "ICANON" feature which causes canonical mode that reads line-by-line on ENTER
    *This will allow to read input byte-by-byte and without the need of pressing ENTER

    3. Removes "ISIG" features which when pressed:
    "Ctrl-C" sends a SIGINT signal to the current process which causes it to terminate
    "Ctrl-Z" sends a SIGTSTP signal to the current process which causes it to suspend

    4. Removes "IXON" feature which when pressed:
    "Ctrl-S" stops data from being transmitted to the terminal until you press "Ctrl-Q"

    5. Removes "IEXTEN" feature which when pressed:
    "Ctrl-V" (ON SOME SYSTEMS) waits for you to type another character and then sends that character literally

    6. Removes "ICRNL" feature which when pressed:
    "Ctrl-M" terminal is helpfully translating any carriage returns (13, '\r') inputted by the user into newlines (10, '\n')

    7. Removes "OPOST" feature which...
*/ 
void enableRawMode() {
    tcgetattr(STDOUT_FILENO, &original_termios);
    atexit(disableRawMode);
    
    /* 
        ~                           (bitwise-NOT operator) flips all bits
        &                           (bitwise-AND operator) returns bits set to 1 if both bits corresponding position are 1
        |                           (bitwise-OR operator) returns bits set to 1 if at least one bits in corresponding position are 1

        c_lflag                     "local flags"
        c_iflag                     "input flags"
        c_oflag                     "output flags"
        c_cflag                     "control flags"
    */
    struct termios raw_termios = original_termios;
    raw_termios.c_iflag &= ~(IXON | ICRNL);
    raw_termios.c_oflag &= ~(OPOST);
    raw_termios.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_termios);
}

// Disabling RAW mode by setting back the original terminal attributes
void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
}