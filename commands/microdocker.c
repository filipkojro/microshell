#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) {

    if (argc < 3) return 1;

    if (strcmp(argv[1], "run") == 0){
        // for (int i = 0; i < argc; i++){
        //     printf("%d=%s\n", i, argv[i]);
        // }
        
        char* container_path = "alpineimage";

        if (chroot(container_path) != 0) {
            perror("chroot failed");
            return 1;
        }
        
        if (chdir("/") != 0) {
            perror("chdir failed");
            return 1;
        }

        char command[256] = "";
        for (int i = 2; i < argc; i++) {
            strcat(command, argv[i]);
            strcat(command, " ");
        }

        printf("running inside container: %s\ncommand: %s\n", container_path, command);

        char *new_argv[] = {"/bin/sh", "-c", command, NULL};
        execv("/bin/sh", new_argv);

        printf("execv failed");
    }
    return 0;
}