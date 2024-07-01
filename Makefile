all: clean build

clean:
	rm -f program

build: main.c
	$(CC) main.c ./source_files/*.c -o program -Wall -Wextra -pedantic -std=c99
