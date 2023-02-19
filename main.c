#include <stdio.h>
#include <stdlib.h>     // atexit(), exit()
#include <errno.h>

#include "./header_files/common.h"
#include "./header_files/raw_mode.h"
#include "./header_files/editor.h"

int main() {
    enableRawMode();

    while(1) {
        editorRefreshScreen();
        editorProcessKeypress();     
    }

    printf("Exiting...\r\n");
    return 0;
}