#include "commands.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CONTINUE 2
#define SUCCESS 1
#define FAIL 0

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
        return FAIL;
    }
    if (chdir(args[1]) != 0) {
        perror("cd");
        return FAIL;
    }

    return CONTINUE;
}

int shellEmpty(char **args) {
    return CONTINUE;
}

Commands commands[] = {
    {"exit", shellExit},
    {"cd", shellCd},
    {"", shellEmpty},
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

    return SUCCESS;
}