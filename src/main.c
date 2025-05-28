#include "../lib/utils.h"
#include "../lib/commands.h"

#include <unistd.h>     // fork, execve, chdir
#include <sys/wait.h>   // wait
#include <limits.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

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

        // check if one of the custom commands was entered
        // if error from one of the custom commands, then skip forking
        if (!checkForCommands(args)) {
            continue;
        }

        int pid = fork();
        if (pid == 0) {
            char path[1024];
            snprintf(path, sizeof(path), "/bin/%s", args[0]);
            execve(path, args, env);
            perror("execve failed");
            exit(0);
        }

        waitpid(pid, &status, 0);
    }

    return 1;
}