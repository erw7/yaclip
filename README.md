# yaclip

Yet another clipboard tool for Windows.

## Rquirements

- [MSYS2]
- [libiconv]

## Installation

Execute msys2\_shell.cmd in the folder where MSYS 2 is installed with the -mingw64 command line option and execute the following commands in the MSYS2 terminal.

    git clone https://github.com/erw7/yaclip.git
    cd yaclip
    mingw32-make

## Basic usage

### Get the clipboard.

    yaclip -o

### Set the clipboard.

    echo "This is a test text." | yaclip  -i

[MSYS2]:https://msys2.github.io/
[libiconv]:https://www.gnu.org/software/libiconv/
