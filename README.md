# Text Editor in C

Simple, open-source text editor written from scratch in C programming language. It provides a minimalist interface with essential features such as text editing, search and replace, syntax highlighting, and more.

## Features

- WORK IN PROGRESS

## Setup

### Requirements

- C compiler (such as `gcc`)
- Build automation tool (such as `make`)

##### Linux

To install the required software on a Linux or Unix based operating system, run the following command:

```bash
sudo apt update
sudo apt-get install gcc make
```

##### macOS

To install the required software on macOS using Homebrew, run the following command:

```bash
brew install gcc make
```

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

This will create an executable file called `program`.

**Note for Windows users:** The `termios.h` header file used by this program is not available on Windows. As a result, you may encounter errors when compiling or running the program on a Windows machine. Consider using a Linux or Mac machine or a virtual machine running Linux to use this program.

## Usage

To use, simply run the executable file from the command line:

```bash
./program
```

To open a file, add an argument next to executable:

```bash
./program random_text_file.txt
```

Once the program is launched, use the arrow keys to navigate and edit text. Use `Ctrl-Q` to quit the program.

## Contributing

This text editor is an open-source project, and everyone is welcome to contribute. If you'd like to contribute, please follow these steps:

1. Fork the project and clone your forked repository to your local machine.
2. Make your changes and commit them to your local repository.
3. Push the changes to your remote repository.
4. Submit a pull request to the master repository.

Please make sure your code follows the project's coding conventions and passes any relevant tests.

## Credits

Foundation was created using the tutorial from [https://viewsourcecode.org/snaptoken/kilo/index.html](https://viewsourcecode.org/snaptoken/kilo/index.html).
