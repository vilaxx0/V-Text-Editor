#include <stdio.h>      // printf(), perror()
#include <stdlib.h>     // atexit(), exit()
#include <errno.h>

#include "./header_files/common.h"
#include "./header_files/raw_mode.h"
#include "./header_files/editor.h"

int main() {
    enableRawMode();
    initEditor();

    while(1) {
        editorRefreshScreen();
        editorProcessKeypress();     
    }

    return 0;
}