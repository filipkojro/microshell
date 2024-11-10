#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define true 1
#define false 0

int cp(argc, argv, envp){
    printf("cp\n");
    return 0;
}

int main(){
    char command[64];
    char arguments[128];

    printf(" zacheta $ ");
    pridtf("%d\n", scanf("%s%[^\n]", command, arguments));

    printf("%s\n%s\n", command, arguments);

    if (fork() != 0){
        printf("rodzic?\n");
        int status;
        waitpid(-1, &status, 0);
        printf("returned with status %d\n", status);
    }
    else {
        printf("dziecko\n");
        // execve(cp, "22", 0);
        return 0;
    }
    return 0;
}
