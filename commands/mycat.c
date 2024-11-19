#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv){

    if (argc < 2){
        printf("mycat command need at least 1 argument\n");
        return 1;
    }

    printf("%s\n",argv[1]);

    char buffer[1024];

    FILE* fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        perror("Could not open file");
        return 1;
    }
    int num;
    while ((num = fread(buffer, 1, sizeof(buffer), fptr)) > 0) {
        // printf("%s", buffer);
        write(STDOUT_FILENO, buffer, num);
    }

    fclose(fptr);
    
    printf("\n");

    return 0;
}
