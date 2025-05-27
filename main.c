#include <unistd.h>     // fork, execve, chdir
#include <sys/wait.h>   // wait

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>     // Support for variadic functions (va_list, va_start, va_end)
#include <string.h>

#define BUFFER_SIZE 1024

// builtin commands for shell
int shell_exit() {
    exit(0);
}

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

Commands commands[] = {
    {"exit", shell_exit},
};

// not sure how to update this yet
char *env[] = { "HOME=/usr/home", "LOGNAME=home", (char *)0 };

int main() {

    // main shell loop
    while (1) {
        print("\n$ ");
        fflush(stdout);

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