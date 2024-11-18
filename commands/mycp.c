#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv){

    if (argc < 3){
        printf("mycp command need at least 2 arguments\n");
        return 1;
    }

    printf("%s\n",argv[1]);

    char buffer[1024];
    int bytes_read;
    // char cwd[4096];

    // if (getcwd(cwd, sizeof(cwd)) == NULL) {
    //     printf("getcwd() error?\n");
    //     return 1;
    // }

    // printf("working dir:%s", cwd);

    FILE* fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        perror("Could not open HTML file");
        return 1;
    }

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fptr)) > 0) {
        printf(buffer);
    }

    fclose(fptr);

    return 0;
}
