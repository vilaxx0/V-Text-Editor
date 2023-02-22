#include <stdio.h>      // printf(), perror()
#include <stdlib.h>     // atexit(), exit()
#include <unistd.h>     // read(), write(), STDIN_FILENO
#include <termios.h>    // struct termios, tcgetattr(), tcsetattr(), TCSAFLUSH, ECHO, ICANON, ISIG, IXON

#include "../header_files/common.h"
#include "../header_files/raw_mode.h"

/*
    1. Removes "ECHO" flag which causes each key press to be printed to the terminal.
    *This is useful in canonical mode, but really gets in the way when we are trying to carefully render a user interface in raw mode.

    2. Removes "ICANON" flag which causes canonical mode that reads line-by-line on ENTER
    *This will allow to read input byte-by-byte and without the need of pressing ENTER

    3. Removes "ISIG" flag which when pressed:
    "Ctrl-C" sends a SIGINT signal to the current process which causes it to terminate
    "Ctrl-Z" sends a SIGTSTP signal to the current process which causes it to suspend

    4. Removes "IXON" flag which when pressed:
    "Ctrl-S" stops data from being transmitted to the terminal until you press "Ctrl-Q"

    5. Removes "IEXTEN" flag which when pressed:
    "Ctrl-V" (ON SOME SYSTEMS) waits for you to type another character and then sends that character literally

    6. Removes "ICRNL" flag which when pressed:
    "Ctrl-M" terminal is helpfully translating any carriage returns (13, '\r') inputted by the user into newlines (10, '\n')

    7. Removes "OPOST" flag which translates each newline ("\n") we print into a carriage return followed by a newline ("\r\n")
    The terminal requires both of these characters in order to start a new line of text. 
    The carriage return moves the cursor back to the beginning of the current line, and the newline moves the cursor down a line, scrolling the screen if necessary.

    8. Removes "BRKINT" flag which on a break condition will cause a SIGINT signal to be sent to the program, like pressing Ctrl-C.
    
    9. Removes "INPCK" flag which enables parity checking, which doesn’t seem to apply to modern terminal emulators.

    10. Removes "ISTRIP" flag which causes the 8th bit of each input byte to be stripped, meaning it will set it to 0. (This is probably already turned off)
    
    11. Removes "CS8" which is NOT a flag, it is a bit mask with multiple bits, which we set using the bitwise-OR (|) operator unlike all the flags we are turning off. 
    It sets the character size (CS) to 8 bits per byte. On my system, it’s already set that way.

    12. "VMIN" value sets the minimum number of bytes of input needed before read() can return. 
    We set it to 0 so that read() returns as soon as there is any input to be read.

    13. "VTIME" value sets the maximum amount of time to wait before read() returns. 
    It is in tenths of a second, so we set it to 1/10 of a second, or 100 milliseconds. 
    If read() times out, it will return 0, which makes sense because its usual return value is the number of bytes read.
*/ 
void enableRawMode() {
    // Open current terminal attributes
    if(tcgetattr(STDOUT_FILENO, &editor.original_termios) == -1)
        die("tcgetattr");

    /* 
        ~                           (bitwise-NOT operator) flips all bits
        &                           (bitwise-AND operator) returns bits set to 1 if both bits corresponding position are 1
        |                           (bitwise-OR operator) returns bits set to 1 if at least one bits in corresponding position are 1

        c_lflag                     "local flags"
        c_iflag                     "input flags"
        c_oflag                     "output flags"
        c_cflag                     "control flags"
    */
    struct termios raw_termios = editor.original_termios;
    raw_termios.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw_termios.c_oflag &= ~(OPOST);
    raw_termios.c_cflag |= (CS8);
    raw_termios.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

    raw_termios.c_cc[VMIN] = 0;
    raw_termios.c_cc[VTIME] = 1;

    // Set current terminal attributes to raw terminal
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_termios) == -1) 
        die("tcsetattr");

    // Automatically called on program exit (exit(0) or exit(1))
    atexit(disableRawMode);
}

// Disabling RAW mode by setting back the original terminal attributes
void disableRawMode() {
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &editor.original_termios) == -1) 
        die("tcsetattr");
}

