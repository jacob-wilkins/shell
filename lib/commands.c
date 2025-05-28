#include "commands.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef int (*builtin_func)(char **args);   // custom type to point to commands

typedef struct {
    const char *name;
    builtin_func func;
} Commands;

// builtin commands for shell
int shellExit(char **args) {
    exit(1);
}

int shellCd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "cd: missing argument\n");
        return 0;
    }
    if (chdir(args[1]) != 0) {
        perror("cd");
        return 0;
    }

    return 0;
}

Commands commands[] = {
    {"exit", shellExit},
    {"cd", shellCd},
};

int numCommands() {
    return sizeof(commands) / sizeof(Commands);
}

int checkForCommands(char **args) {
    for(int i = 0; i < numCommands(); i++) {
        if (strcmp(args[0], commands[i].name) == 0) {
            return commands[i].func(args);
        }
    }

    return 1;
}