all: clean build launch

clean:
	rm -f vte

build: main.c
	$(CC) main.c editor.c -o vte -Wall -Wextra -pedantic -std=c99

launch:
	./vte