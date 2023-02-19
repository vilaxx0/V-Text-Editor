#include <stdio.h>      // printf(), perror()
#include <stdlib.h>     // atexit(), exit()

void die(const char* s) {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
    
    perror(s);
    exit(1);
}