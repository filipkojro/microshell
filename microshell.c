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
    // printf("%s\n\n", cwd);
    

    printf("%s $ ", cwd);
    scanf("%s%[^\n]", command, arguments);

    printf("command:%s\narguments:%s\n", command, arguments);

    if (fork() != 0){
        // printf("rodzic?\n");

        int status;
        waitpid(-1, &status, 0);

        printf("returned with status %d\n", status);
    }
    else {
        char* path_to_command = concat("./commands/", command);
        char *args[] = {"./", NULL};

        char ar[100];
        char rest[100];
        char* temp = arguments;

        for (int i = 0; i < 20; i++){
            if (strlen(temp) <= 0) break;
            sscanf(temp, " %s", ar);

            temp += sizeof(char) * (strlen(ar) + 1);
            printf("%dnxtarg:%s\n", i, ar);
        }

        // execv(path_to_command, args);

        execvp("ls", args);

        printf("nie bylo takiej komendy\n");

        free(path_to_command);
        return 0;
    }
    return 0;
}
