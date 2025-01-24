#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#define true 1
#define false 0

// colors using ANSI escape code \033[1;{color code}m
#define BLINK "\001\033[5m\002"
#define RED_TEXT "\001\033[1;31m\002"
#define GREEN_TEXT "\001\033[1;32m\002"
#define BLUE_TEXT "\001\033[1;34m\002"
#define RESET_TEXT "\001\033[1;0m\002"
int sigint2;



char* concat(const char *s1, const char *s2) {
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1); // +1 for terminator
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1); // +1 to copy terminator
    return result;
}

int gen_prompt(char* prompt){
    char whole_cwd[4096];
    char cwd[4096];
    char* user = getlogin();
    char hostname[1024];

    if (gethostname(hostname, sizeof(hostname)) != 0){
        printf(RED_TEXT"gethostname() error?\n");
        return 1;
    }

    if (getcwd(whole_cwd, sizeof(whole_cwd)) == NULL) {
        printf(RED_TEXT"getcwd() error?\n");
        return 1;
    }

    char* home_dir;
    if ((home_dir = getenv("HOME")) == NULL){
        printf(RED_TEXT"HOME error?\n");
        return 1;
    }

    char possible_home[4096];

    if (strlen(home_dir) <= strlen(whole_cwd)){
        strncpy(possible_home, whole_cwd, strlen(home_dir));

        if (strcmp(possible_home, home_dir) == 0){
            strcpy(cwd, concat("~", whole_cwd + strlen(home_dir)));
        }
        else {
            strcpy(cwd, whole_cwd);
        }
    }
    else {
        strcpy(cwd, whole_cwd);
    }

    snprintf(prompt, 4096, RESET_TEXT"!microshell!"BLUE_TEXT"%s@%s "GREEN_TEXT"%s"RESET_TEXT" $ ", user, hostname, cwd);
    return 0;
}

void handle_sigint(int sig) {
    if(sigint2){
        return;
    }
    else{
        char prompt[8192];
        gen_prompt(prompt);
        printf("\n%s", prompt);
    }
}

int mycd(int argc, char **argv){
    printf("%s\n", argv[0]);
    printf("%s\n", argv[1]);

    char* direcotry = getenv("HOME");

    if (argc != 0){
        direcotry = argv[1];
    }
    if (chdir(direcotry) != 0){
        printf("problem with chdir\n");
        return 1;
    }
    return 0;
}

int main(){
    signal(SIGINT, handle_sigint);

    char cwd[4096];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        printf(RED_TEXT"getcwd() error?\n");
        return 1;
    }
    setenv("PATH", concat(concat(cwd, "/commands/bin:"), getenv("PATH")), 1);

    printf("%s", concat(cwd, "/commands/bin:"));

    char* inp;
    char command[32];
    char arguments[1024];

    char prompt[8192];

    return 0;

    while (1){ 
        
        strcpy(command, "");
        strcpy(arguments, "");

        // fgets(inp, 4096, stdin);
        if(gen_prompt(prompt) == 1){
            exit(1);
        }
        sigint2 = 0;
        // inp = readline(prompt);
        sigint2 = 1;

        // add_history(inp);

        sscanf(inp, "%s%[^\n]", command, arguments);

        // counting arguments
        int argc = 0;
        int last_space = 0;
        int c;
        int argument_problem = 0;
        // printf("arguments:%s:", arguments);

        for (c = 0; c < strlen(arguments); c++){
            if (arguments[c] == ' ') {
                if (last_space != 1){
                    argc++;
                    last_space = 1;
                }
            }
            else last_space = 0;
        }
        if (last_space == 1){
            argc--;
        }
        if (argument_problem == 1){
            continue;
        }

        char args[argc + 1][1024];
        char* argv[argc + 2];

        strcpy(args[0], command);
        
        char ar[1024];
        char rest[1024];
        char rest2[1024];
        
        strcpy(rest, arguments);
        
        int i;

        //inserting argument to vector
        
        for (i = 1; i < argc + 1; i++){
            
            sscanf(rest, " %s%[^\0]", ar, rest2);
            
            strcpy(args[i], ar);
            // printf("some text\n");
            strcpy(rest, rest2);
        }

        for (i = 0; i < argc + 1; i++){
            argv[i] = args[i];
        }
        argv[argc + 1] = NULL;

        // for (i = 0; i < argc + 2; i++){
        //     printf("i:%d:%s\n", i, argv[i]);
        // }

        if (strcmp(command, "exit") == 0){
            printf("leaving :)\n");
            exit(0);
        }
        else if (strcmp(command, "mycd") == 0){
            if (mycd(argc, argv) != 0){
                printf("proglem with mycd\n");
            }
            continue;
            
        }

        int childpid;

        if ((childpid = fork()) != 0){
            // printf("rodzic?\n");

            int status;
            wait(&status);

            if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                printf("Error number: %d\n", WEXITSTATUS(status));
            }
        }
        else {
            signal(SIGINT, SIG_DFL);

            execvp(command, argv);
            printf(RED_TEXT"nie ma takiej komendy\n");
            _exit(errno);

            return 0;
        }
    }

    
    return 0;
}
