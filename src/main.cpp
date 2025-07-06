#include "../lib/utils.h"
#include "../lib/commands.h"

#include <unistd.h>     // fork, execve, chdir
#include <sys/wait.h>   // wait
#include <limits.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

#define SUCCESS 2
#define COMMAND_NOT_FOUND 1
#define FAIL 0

// not sure how to update this yet
// might change to using execvp instead of execve
char *env[] = { "HOME=/usr/home", "LOGNAME=home", (char *)0 };

int main() {
    char buf[BUFFER_SIZE];
    char hostname[BUFFER_SIZE];

    // main shell loop
    while (1) {
        char *cwd = getcwd(buf, (size_t)pathconf(".", _PC_PATH_MAX));
        char *home = getenv("HOME");
        char display_cwd[BUFFER_SIZE];
        gethostname(hostname, sizeof(hostname));

        if (home && strncmp(cwd, home, strlen(home)) == 0) {
            // Replace home directory with ~
            snprintf(display_cwd, sizeof(display_cwd), "~%s", cwd + strlen(home));
        } else {
            snprintf(display_cwd, sizeof(display_cwd), "%s", cwd);
        }
        
        print("%s@%s:%s$ ", getenv("USER"), hostname, display_cwd);

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
        switch (checkForCommands(args)) {
            case COMMAND_NOT_FOUND:
                // no custom command was found, so now its
                // time to rely on execve
                // kinda confusing naming
                // might change in the future
                break;
            default:
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

    return FAIL;
}