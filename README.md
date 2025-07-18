
# Simple Shell

This project is a simple Unix-like shell implemented in C. It supports running external commands, as well as a set of built-in commands.

## Features

- Execute external programs (e.g., `ls`, `echo`, etc.)
- Built-in commands:
  - `exit` — Exit the shell
  - `cd <directory>` — Change the current working directory
- Command-line prompt
- Basic argument parsing (space-delimited)

## Dependencies

- gcc
- make
- POSIX-complient OS (in order to fork and execvp)
- gnome-terminal

## BEFORE RUNNING PROJECT

To get all of the dependencies for this project, run:

```sh
sudo apt-get update
sudo apt-get install build-essential gnome-terminal
```

## Build
To build the shell, use the provided Makefile:

```sh
make
```

## Usage

Run the shell:

```sh
make run
```

## Examples

```sh
$ ls
$ cd /tmp
$ echo Hello, world!
Hello, world!
$ exit
```
## Custom Commands

You can add more built-in commands by editing the `commands` table and providing handler functions in `lib/commands.c`.

## Cleaning Up
To remove compiled files:

```sh
make clean
```
## Notes
- This shell is for educational purposes and does not implement advanced features like pipes, redirection, or job control.
- Only basic error handling is provided.


Feel free to extend this shell with more features!
