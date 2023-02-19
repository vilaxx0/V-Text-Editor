#include <stdio.h>      // printf(), perror()
#include <stdlib.h>     // atexit(), exit()
#include <unistd.h>     // read(), write(), STDIN_FILENO, STDOUT_FILENO
#include <errno.h>

#include "../header_files/common.h"
#include "../header_files/editor.h"

char editorReadKey() {
    char c;
    int nread;
    while((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if(nread == -1 && errno != EAGAIN) {
            die("read");
        }
    }
    return c;
}

void editorProcessKeypress() {
    char c = editorReadKey();

    switch (c) {
        case CTRL_KEY('q'):
            write(STDOUT_FILENO, "\x1b[2J", 4);
            write(STDOUT_FILENO, "\x1b[H", 3);
            exit(0);
            break;
        default:
            printf("%d\r\n", c);
    }
}

void editorRefreshScreen() {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
}