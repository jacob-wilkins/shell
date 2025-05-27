#include <unistd.h>     // fork, execve, chdir
#include <sys/wait.h>   // wait
#include <limits.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>     // Support for variadic functions (va_list, va_start, va_end)
#include <string.h>

#define BUFFER_SIZE 1024

static void print(const char* format, ...) {
    char buffer[BUFFER_SIZE];   // Buffer to store formatted output
    va_list args;   // Holds all the arguments after 'format'
    
    va_start(args, format);     // Initialize the va_list to start processing the arguments after 'format'

    /* How to use vsnprintf(s, n, format, arg):
        - s: buffer to store formatted string
        - n: maximum number of bytes in the buffer
        - format: String that contains a format string
        - args: va_list that contains all the arguments after the formatted string
          can only be used after va_start is called, and until va_end is called
    */
    vsnprintf(buffer, BUFFER_SIZE, format, args);

    va_end(args);   // Clean up the va_list when done processing the arguments

    write(STDOUT_FILENO, buffer, strlen(buffer));
    fflush(stdout);  // Ensure output is written immediately
}

typedef int (*builtin_func)(char **args);   // custom type to point to commands

typedef struct {
    const char *name;
    builtin_func func;
} Commands;

// builtin commands for shell
int shellExit(char **args) {
    exit(0);
}

void shellCd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "cd: missing argument\n");
    }
    if (chdir(args[1]) != 0) {
        perror("cd");
    }
}

Commands commands[] = {
    {"exit", shellExit},
    {"cd", shellCd},
};

int numCommands() {
    return sizeof(commands) / sizeof(Commands);
}

void checkForCommands(char **args) {
    for(int i = 0; i < numCommands(); i++) {
        if (strcmp(args[0], commands[i].name) == 0) {
            commands[i].func(args);
        }
    }
}

// not sure how to update this yet
char *env[] = { "HOME=/usr/home", "LOGNAME=home", (char *)0 };

int main() {
    char buf[BUFFER_SIZE];

    // main shell loop
    while (1) {
        char *cwd = getcwd(buf, (size_t)pathconf(".", _PC_PATH_MAX));
        print("\n%s$ ", cwd);

        int status;

        // get input from command line
        char line[1024];
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }

        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;

        // Tokenize input into arg array
        char* args[64];
        int iter = 0;
        char* token = strtok(line, " ");
        while (token && iter < 63) {
            args[iter++] = token;
            token = strtok(NULL, " ");
        }
        args[iter] = NULL;

        // TODO: check if args[0] is in the custom command list
        //          if so, then execute the command
        //          else, fork
        checkForCommands(args);

        int pid = fork();
        if (pid == 0) {
            char path[1024];
            snprintf(path, sizeof(path), "/bin/%s", args[0]);
            execve(path, args, env);
            perror("execve failed");
            exit(1);
        }

        waitpid(pid, &status, 0);
    }

    return 0;
}