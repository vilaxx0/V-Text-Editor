#include "editor.h"

int main() {
    enableRawMode();

    // Read 1 byte from the standard input into the variable c
    // Keep doing it until there are no more bytes to read (return 0 when reaches end of the file)
    char c;
    while(read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
        if(iscntrl(c)) {
            printf("%d\n", c);
        } else {
            printf("%d ('%c')\n", c, c);
        }
    }

    printf("Exiting...\n");
    return 0;
}