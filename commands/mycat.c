#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void parse_directory(char* output_direcotry, char* input_directory){
    char* home_dir = getenv("HOME");

    if (input_directory != 0){
        if (input_directory[0] == '~'){
            sprintf(output_direcotry, "%s%s", home_dir, input_directory + 1);
        }
        else {
            strcpy(output_direcotry, input_directory);
        }
    }
}

int main(int argc, char **argv){

    if (argc < 2){
        printf("mycat command need at least 1 argument\n");
        return 1;
    }

    // printf("%s\n",argv[1]);

    char directory[4096];

    parse_directory(directory, argv[1]);

    char buffer[1024];

    FILE* fptr = fopen(directory, "r");
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
