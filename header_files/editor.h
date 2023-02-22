#ifndef EDITOR_H
#define EDITOR_H

#define ABUF_INIT {NULL, 0}
#define VTE_VERSION "0.0.1"

enum editorKey {
  ARROW_LEFT = 1000,
  ARROW_RIGHT,
  ARROW_UP,
  ARROW_DOWN,
  PAGE_UP,
  PAGE_DOWN
};

struct ABuf {
  char *b;
  int len;
};

void editorMoveCursor(int key);
void initEditor();
int editorReadKey();
void editorProcessKeypress();
void editorRefreshScreen();
void editorDrawRows(struct ABuf *aBuf);
int getWindowSize(int* rows, int* cols);
int getCursorPosition(int* rows, int* cols);
void aBufAppend(struct ABuf* aBuf, const char* s, int len);
void aBufFree(struct ABuf* aBuf);

#endif