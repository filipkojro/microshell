#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define true 1
#define false 0

// colors using ANSI escape code \033[1;{color code}m
#define BLINK "\033[5m"
#define RED_TEXT "\033[1;31m"
#define GREEN_TEXT "\033[1;32m"
#define BLUE_TEXT "\033[1;34m"
#define RESET_TEXT "\033[1;0m"

char* concat(const char *s1, const char *s2) {
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1); // +1 for terminator
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1); // +1 to copy terminator
    return result;
}

int main(){
    setenv("PATH", concat("/Users/filip/Sync/UAM/SO/microshell/commands/bin:", getenv("PATH")), 1);
    // printf("%s\n", getenv("PATH"));

    char command[32];
    char arguments[1024];

    char cwd[4096];
    char* user = getlogin();
    char hostname[1024];

    if (gethostname(hostname, sizeof(hostname)) != 0){
        printf(RED_TEXT"gethostname() error?\n");
        return 1;
    }

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        printf(RED_TEXT"getcwd() error?\n");
        return 1;
    }

    printf(BLUE_TEXT"%s@%s "GREEN_TEXT"%s"RESET_TEXT" $ ", user, hostname, cwd);
    scanf("%s%[^\n]", command, arguments);

    if (fork() != 0){
        // printf("rodzic?\n");

        int status;
        waitpid(-1, &status, 0);

        if (status != 0){
            printf("reteturned with status: %d\n", status);
        }
    }
    else {
        int argc = 0;

        int c;
        for (c = 0; c < strlen(arguments); c++){
            if (arguments[c] == ' ') argc++;
        }
        if (c == 0){
            arguments[0] = ' ';
            arguments[1] = '\0';
            argc++;
        }
        
        char *args[argc + 2];
        args[0] = command;
        args[argc] = NULL;

        char ar[100];
        char* temp_args = arguments;
        for (int i = 1; i < argc+1; i++){
            if (strlen(temp_args) <= 0) break;
            sscanf(temp_args, " %s", ar);
            args[i] = ar;

            temp_args += sizeof(char) * (strlen(ar) + 1);
            // printf("%dnxtarg:%s\n", i, ar);
        }

        execvp(command, args);

        printf(RED_TEXT"nie ma takiej komendy\n");

        return 0;
    }
    return 0;
}
