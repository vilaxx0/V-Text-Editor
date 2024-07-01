#include <stdio.h>      // printf(), perror(), FILE, fopen(), getline()
#include <stdlib.h>     // atexit(), exit()
#include <errno.h>

#include "./header_files/common.h"
#include "./header_files/raw_mode.h"
#include "./header_files/editor.h"

int main(int argc, char *argv[]) {
    enableRawMode();
    initEditor();
    if (argc >= 2) {
        openEditor(argv[1]);
    }
    
    while (1) {
        editorRefreshScreen();
        editorProcessKeypress();     
    }

    return 0;
}