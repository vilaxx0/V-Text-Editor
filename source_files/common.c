#include <stdio.h>      // printf(), perror()
#include <stdlib.h>     // atexit(), exit()
#include <unistd.h>     // read(), write(), STDIN_FILENO, STDOUT_FILENO

#include "../header_files/common.h"

struct EditorConfig editor;

/*** terminal ***/

void die(const char* s) {
    write(STDOUT_FILENO, "\x1b[2J", 4); // clear screen
    write(STDOUT_FILENO, "\x1b[H", 3);  // reposition the cursor
    
    perror(s);
    exit(1);
}