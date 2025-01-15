#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv){

    if (argc < 3){
        printf("mycp command need at least 2 arguments\n");
        return 1;
    }

    // printf("%s\n",argv[0]);
    // printf("%s\n",argv[1]);
    // printf("%s\n",argv[2]);

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

    char buffer[1024];

    if (infptr == NULL) {
        perror("Could not open file");
        return 1;
    }
    int num;
    while ((num = fread(buffer, 1, sizeof(buffer), infptr)) > 0) {
        fwrite(buffer, 1, num, outfptr);
    }

    fclose(infptr);
    fclose(outfptr);

    return 0;
}
