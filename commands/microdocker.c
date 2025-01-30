#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <string.h>

int child(void* arg){
    char* container_path = "alpineimage";

    if (chroot(container_path) != 0) {
        perror("chroot failed: u need to run as root");
        return 1;
    }
    
    if (chdir("/") != 0) {
        perror("chdir failed: u need to run as root");
        return 1;
    }

    if (mount("proc", "/proc", "proc", 0, NULL) != 0) {
        perror("mount failed: u need to run as root");
        return 1;
    }

    sethostname("container", sizeof("container"));

    char **args = (char **)arg;
    char command[256] = "";
    for (int i = 2; args[i] != NULL; i++) {
        strcat(command, args[i]);
        strcat(command, " ");
    }

    printf("running inside container: %s\ncommand: %s\n", container_path, command);

    char *new_argv[] = {"/bin/sh", "-c", command, NULL};
    execv("/bin/sh", new_argv);

    printf("execv failed");
    return 1;
}

int main(int argc, char **argv) {

    if (argc < 3) {
        printf("za malo argumentow\npoprawne wykonanie: sudo ./commands/bin/microdocker run [COMMAND] [ARGUMENTS]\n");
        return 1;
    }

    if (strcmp(argv[1], "run") == 0){
        
        const int STACK_SIZE = 1024 * 1024;  // 1MB stack for the child
        void *stack = malloc(STACK_SIZE);
        if (!stack) {
            perror("malloc failed");
            return 1;
        }

        int flags = CLONE_NEWUTS | CLONE_NEWPID | SIGCHLD;
        
        pid_t pid = clone(child, stack + STACK_SIZE, flags, argv);
        if (pid == -1) {
            perror("clone failed");
            free(stack);
            return 1;
        }
        waitpid(pid, NULL, 0);
        free(stack);
    }
    else {
        printf("curretly only run is implemented\n");
        return 1;
    }
    return 0;
}