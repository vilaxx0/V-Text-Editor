#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include <stdio.h>      // printf(), perror(), sscanf(), snprintf()
#include <stdlib.h>     // atexit(), exit(), malloc(), realloc(), free()
#include <unistd.h>     // read(), write(), STDIN_FILENO, STDOUT_FILENO
#include <errno.h>      // errno
#include <sys/ioctl.h>  // ioctl(), TIOCGWINSZ, struct winsize
#include <sys/types.h>  // ssize_t
#include <ctype.h>      // iscntrl()
#include <string.h>     // memcpy(), strlen()

#include "../header_files/common.h"
#include "../header_files/editor.h"

/*** init ***/

void editorInit() {
    editor.cursorX = 0;
    editor.cursorY = 0;
    editor.rX = 0;

    editor.nrOfRows = 0;
    editor.rowOffset = 0;
    editor.colOffset = 0;
    editor.row = NULL;

    if(getWindowSize(&editor.screenRows, &editor.screenCols) == -1) 
        die("getWindowSize");
}

/*** file i/o ***/

void editorOpen(char* filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) die("fopen");
    
    char *line = NULL;
    size_t lineCapacity = 0;
    ssize_t lineLength = getline(&line, &lineCapacity, fp);
    while ((lineLength = getline(&line, &lineCapacity, fp)) != -1) {
        while (lineLength > 0 && (line[lineLength - 1] == '\n' || line[lineLength - 1] == '\r')) {
            lineLength--;
        }

        editorAppendRow(line, lineLength);
    }

    free(line);
    fclose(fp);
}

/*** terminal ***/

int editorReadKey() {
    char c;
    int nread;
    while((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if(nread == -1 && errno != EAGAIN)
            die("read");
    }

    // If read key is an escape sequence
    if(c == '\x1b') {
        char seq[3];
        
        if(read(STDIN_FILENO, &seq[0], 1) != 1)
            return '\x1b';

        if(read(STDIN_FILENO, &seq[1], 1) != 1)
            return '\x1b';

        if(seq[0] == '[') {
            if(seq[1] >= '0' && seq[1] <= '9') {
                if(read(STDIN_FILENO, &seq[2], 1) != 1)
                    return '\x1b';
                
                if(seq[2] == '~') {
                    switch (seq[1]) {
                        case '1':
                            return HOME_KEY;
                        case '3':
                            return DELETE_KEY;
                        case '4':
                            return END_KEY;
                        case '5':
                            return PAGE_UP;
                        case '6':
                            return PAGE_DOWN;
                        case '7':
                            return HOME_KEY;
                        case '8':
                            return END_KEY;
                    }
                }
            } else {
                switch (seq[1]) {
                    case 'A':
                        return ARROW_UP;
                    case 'B':
                        return ARROW_DOWN;
                    case 'C':
                        return ARROW_RIGHT;
                    case 'D':
                        return ARROW_LEFT;
                    case 'H':
                        return HOME_KEY;
                    case 'F':
                        return END_KEY;
                }
            }
        } 

        if(seq[0] == 'O') {
            switch (seq[1]) {
            case 'H':
                return HOME_KEY;
            case 'F':
                return END_KEY;
            }
        }

        return '\x1b';
    } else {
        return c;
    }
}

/*** input ***/

void editorProcessKeypress() {
    int c = editorReadKey();

    switch (c) 
    {
        case CTRL_KEY('q'):
            write(STDOUT_FILENO, "\x1b[2J", 4);
            write(STDOUT_FILENO, "\x1b[H", 3);
            exit(0);
            break;

        case ARROW_UP:
        case ARROW_DOWN:
        case ARROW_LEFT:
        case ARROW_RIGHT:
            editorMoveCursor(c);
            break;

        case HOME_KEY:
            editor.cursorX = 0;
            break;

        case END_KEY:
            editor.cursorX = editor.screenCols - 1;
            break;

        case PAGE_UP:
        case PAGE_DOWN:
            {
                if (c == PAGE_UP) {
                    editor.cursorY = editor.rowOffset;
                } else if (c == PAGE_DOWN) {
                    editor.cursorY = editor.rowOffset + editor.screenRows - 1;
                    if (editor.cursorY > editor.nrOfRows) {
                        editor.cursorY = editor.nrOfRows;
                    }
                }

                int times = editor.screenRows;
                while(times--)
                    editorMoveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
            }
            break;
    }
}

void editorMoveCursor(int key) {
    struct EditorRow *row = (editor.cursorY >= editor.nrOfRows) ? NULL : &editor.row[editor.cursorY];

    switch (key) {
        case ARROW_LEFT:
            if(editor.cursorX != 0) {
                editor.cursorX--;
            } else if (editor.cursorY > 0) {
                editor.cursorY--;
                editor.cursorX = editor.row[editor.cursorY].size;
            }
            break;
        case ARROW_RIGHT:
            if (row && editor.cursorX < row->size) {
                editor.cursorX++;
            } else if (row && editor.cursorX == row->size) {
                editor.cursorY++;
                editor.cursorX = 0;
            }
            break;
        case ARROW_UP:
            if(editor.cursorY != 0) {
                editor.cursorY--;
            }
            break;
        case ARROW_DOWN:
            if(editor.cursorY < editor.nrOfRows) {
                editor.cursorY++;
            }
            break;
    }

    row = (editor.cursorY >= editor.nrOfRows) ? NULL : &editor.row[editor.cursorY];
    int rowLength = row ? row->size : 0;

    if (editor.cursorX > rowLength) {
        editor.cursorX = rowLength;
    }
}

int getWindowSize(int* rows, int* cols) {
    struct winsize ws;

    if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        // Fallback method to get windows size if ioctl fails
        if(write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) 
            return -1;

        return getCursorPosition(rows, cols);
    } else {
        *rows = ws.ws_row;
        *cols = ws.ws_col;
        return 0;
    }
}

int getCursorPosition(int* rows, int* cols) {
    char buf[32];

    if(write(STDOUT_FILENO, "\x1b[6n", 4) != 4) 
        return -1;

    unsigned int i = 0;
    while(i < (sizeof(buf) - 1)) {
        if(read(STDIN_FILENO, &buf[i], 1) != 1) 
            break;

        if(buf[i] == 'R') 
            break;
        
        i++;
    }

    buf[i] = '\0';
    
    if(buf[0] != '\x1b' || buf[1] != '[') 
        return -1;

    if(sscanf(&buf[2], "%d;%d", rows, cols) != 2) 
        return -1;

    return 0;
}

/*** output ***/

void editorRefreshScreen() {
    editorScroll();

    struct ABuf aBuf = ABUF_INIT;

    // Hide cursor
    aBufAppend(&aBuf, "\x1b[?25l", 6);

    // Clear the entire screen
    // aBufAppend(&aBuf, "\x1b[2J", 4); 

    // Cursor position (reposition the cursor back up at the top-left corner)
    aBufAppend(&aBuf, "\x1b[H", 3);

    editorDrawRows(&aBuf);

    // Reposition cursor
    char buf[23];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", (editor.cursorY - editor.rowOffset) + 1, (editor.rX - editor.colOffset) + 1);
    aBufAppend(&aBuf, buf, strlen(buf));

    // Show cursor
    aBufAppend(&aBuf, "\x1b[?25h", 6);

    write(STDOUT_FILENO, aBuf.b, aBuf.len);
    aBufFree(&aBuf);
}

void editorDrawRows(struct ABuf *aBuf) {
    for (int i = 0; i < editor.screenRows; i++) {
        int fileRow = i + editor.rowOffset;
        if (fileRow >= editor.nrOfRows) {
            if (i == 1 && editor.nrOfRows == 0) {
                char welcome[80];
                int welcomeLength = snprintf(welcome, sizeof(welcome), "VTE editor -- version %s", VTE_VERSION);

                if(welcomeLength > editor.screenCols)
                    welcomeLength = editor.screenCols;

                int padding = (editor.screenCols - welcomeLength) / 2;
                if(padding) {
                    aBufAppend(aBuf, "~", 1);
                    padding--;
                }

                while(padding--)
                    aBufAppend(aBuf, " ", 1);

                aBufAppend(aBuf, welcome, welcomeLength);
                
            } else {
                aBufAppend(aBuf, "~", 1);
            }
        } else {
            int length = editor.row[fileRow].rSize - editor.colOffset;

            if (length < 0)
                length = 0;

            if (length > editor.screenCols)
                length = editor.screenCols;

            aBufAppend(aBuf, &editor.row[fileRow].renderChars[editor.colOffset], length);
        }
        
        // Erase part of the current line (erases the part of the line to the right)
        aBufAppend(aBuf, "\x1b[K", 3);

        if(i < editor.screenRows - 1) 
            aBufAppend(aBuf, "\r\n", 2);
    }
}

void editorScroll() {
    editor.rX = 0;

    if (editor.cursorY < editor.nrOfRows) {
        editor.rX = editorRowCxToRx(&editor.row[editor.cursorY], editor.rX);
    }

    if (editor.cursorY < editor.rowOffset) {
        editor.rowOffset = editor.cursorY;
    }

    if (editor.cursorY >= editor.rowOffset + editor.screenRows) {
        editor.rowOffset = editor.cursorY - editor.screenRows + 1;
    }

    if (editor.rX < editor.colOffset) {
        editor.colOffset = editor.rX;
    }
    
    if (editor.rX >= editor.colOffset + editor.screenCols) {
        editor.rowOffset = editor.rX - editor.screenCols + 1;
    }
}

/*** append buffer ***/

void aBufAppend(struct ABuf* aBuf, const char* s, int len) {
    char *new = realloc(aBuf->b, aBuf->len + len);

    if(new == NULL) 
        return;

    memcpy(&new[aBuf->len], s, len);
    aBuf->b = new;
    aBuf->len += len;
}

void aBufFree(struct ABuf* aBuf) {
    free(aBuf->b);
}

/*** row operations ***/

void editorAppendRow(char *s, size_t length) {
    editor.row = realloc(editor.row, sizeof(EditorRow) * (editor.nrOfRows + 1));

    int pos = editor.nrOfRows;
    editor.row[pos].size = length;
    editor.row[pos].chars = malloc(length + 1);
    memcpy(editor.row[pos].chars, s, length);
    editor.row[pos].chars[length] = '\0';

    editor.row[pos].rSize = 0;
    editor.row[pos].renderChars = NULL;
    editorUpdateRow(&editor.row[pos]);
    
    editor.nrOfRows++;
}

void editorUpdateRow(EditorRow *row) {
    int tabs = 0;
    for (int i = 0; i < row->size; i++) {
        if (row->chars[i] == '\t') {
            tabs++;
        }
    }
    free(row->renderChars);
    row->renderChars = malloc(row->size * tabs * (KILO_TAB_STOP - 1) + 1);

    int idx = 0;
    for (int j = 0; j < row->size; j++) {
        if (row->chars[j] == '\t') {
            row->renderChars[idx++] = ' ';
            while (idx % KILO_TAB_STOP != 0) {
                row->renderChars[idx++] = ' ';
            }
        } else {
            row->renderChars[idx++] = row->chars[j];
        }
    }

    row->renderChars[idx] = '\0';
    row->rSize = idx;
}

int editorRowCxToRx(EditorRow *row, int cursorX) {
    int rx = 0;
    for (int i = 0; i < cursorX; i++) {
        if (row->chars[i] == '\t')
        rx += (KILO_TAB_STOP - 1) - (rx % KILO_TAB_STOP);
        rx++;
    }
    return rx;
}