# Vilo Text Editor (VTE)

Vilo Text Editor (VTE) is a simple, open-source text editor written from scratch in C programming language. It provides a minimalist interface with essential features such as text editing, search and replace, syntax highlighting, and more.

**WORK IN PROGRESS**

## Features

- Syntax highlighting for several programming languages.
- Tabbed interface for easy navigation between multiple files.
- Search and replace functionality.
- Cut, copy, and paste support.
- Undo and redo functionality.
- Open, save, and close files.

## Setup

### Requirements

- C Compiler (such as gcc)
- Linux or Unix based operating system.

### Installation

1. Clone the repository to your local machine:

```bash
https://github.com/vilaxx0/ViloTextEditor.git
```

2. Change to the project directory:

```bash
cd ViloTextEditor
```

3. Compile the project:

```bash
make
```

This will create an executable file called `vte`.

**Note for Windows users:** The `termios.h` header file used by this program is not available on Windows. As a result, you may encounter errors when compiling or running the program on a Windows machine. Consider using a Linux or Mac machine or a virtual machine running Linux to use this program.

## Usage

To use VTE, simply run the executable file from the command line:

```bash
./vte
```

Once the program is launched, use the arrow keys to navigate and edit text. Use `Ctrl-Q` to quit the program.

## Contributing

Vilo Text Editor is an open-source project, and everyone is welcome to contribute. If you'd like to contribute, please follow these steps:

1. Fork the project and clone your forked repository to your local machine.
2. Make your changes and commit them to your local repository.
3. Push the changes to your remote repository.
4. Submit a pull request to the master repository.

Please make sure your code follows the project's coding conventions and passes any relevant tests.

## Credits

Vilo Text Editor was created using the tutorial from [https://viewsourcecode.org/snaptoken/kilo/index.html](https://viewsourcecode.org/snaptoken/kilo/index.html).
