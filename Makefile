all: clean build

clean:
	rm -f vte

build: main.c
	$(CC) main.c editor.c -o vte -Wall -Wextra -pedantic -std=c99
