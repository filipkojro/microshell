#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define true 1
#define false 0

char* concat(const char *s1, const char *s2) {
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1); // +1 for the null-terminator
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1); // +1 to copy the null-terminator
    return result;
}

int main(){
    char command[64];
    char arguments[128];

    char cwd[4096];

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        printf("getcwd() error?\n");
        return 1;
    }

    printf("%s $ ", cwd);
    scanf("%s%[^\n]", command, arguments);

    // printf("command:%s\narguments:%s\n", command, arguments);

    if (fork() != 0){
        // printf("rodzic?\n");

        int status;
        waitpid(-1, &status, 0);

        if (status != 0){
            printf("returned with status %d\n", status);
        }
    }
    else {
        int argc = 0;
        for (int c = 0; c < strlen(arguments); c++){
            if (arguments[c] == ' ') argc++;
        }     

        char *args[argc + 1];
        args[argc] = NULL;

        char ar[100];
        char* temp_args = arguments;
        for (int i = 0; i < argc; i++){
            if (strlen(temp_args) <= 0) break;
            sscanf(temp_args, " %s", ar);
            args[i] = ar;

            temp_args += sizeof(char) * (strlen(ar) + 1);
            // printf("%dnxtarg:%s\n", i, ar);
        }

        execvp(command, args);

        printf("nie bylo takiej komendy\n");

        return 0;
    }
    return 0;
}
