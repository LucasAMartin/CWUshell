# CWUShell

CWUshell is a custom Linux shell developed for CS470 in C.

## Features

CWUShell supports the following commands:

- `exit`: Exits the shell.
- `prompt`: Customizes the command prompt.
- `fileinfo`: Provides information about a file.
- `osinfo`: Provides information about the operating system.

## Usage

To use CWUShell, compile the source code and then run the resulting executable. You can then enter commands at the prompt.

### Exit Command

The `exit` command allows you to exit the shell. Simply type `exit` at the prompt and press enter.

### Prompt Command

The `prompt` command allows you to customize the command prompt. To use it, type `prompt` followed by the text you want to display as your prompt.

### FileInfo Command

The `fileinfo` command provides information about a file. It supports the following switches:

- `-i`: Print the inode number of the file.
- `-t`: Print the type of the file.
- `-m`: Print the last modification date of the file.

If no switch is specified, all three pieces of information are printed.

### OSInfo Command

The `osinfo` command provides information about the operating system. It supports the following switches:

- `-s`: Print the operating system name.
- `-v`: Print the operating system version.
- `-a`: Print the computer architecture.

### Help Command

All of the custom commands support -h and --help switches to get miore information on usage
