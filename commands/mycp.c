#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv){

    if (argc < 3){
        printf("mycp command need at least 2 arguments\n");
        return 1;
    }

    printf("%s\n",argv[0]);
    printf("%s\n",argv[1]);
    printf("%s\n",argv[2]);

    char buffer[1024];
    int bytes_read;

    return 0;

    FILE* infptr = fopen(argv[1], "r");

    FILE* outfptr = fopen(argv[2], "w");



    if (infptr == NULL) {
        perror("Could not open file");
        return 1;
    }
    if (outfptr == NULL) {
        perror("Could not open 22222 file");
        return 1;
    }

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), infptr)) > 0) {
        printf(buffer);
    }

    fclose(infptr);

    return 0;
}
