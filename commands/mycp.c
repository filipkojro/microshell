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

    if (argc < 3){
        printf("mycp command need at least 2 arguments\n");
        return 1;
    }

    // printf("%s\n",argv[0]);
    // printf("%s\n",argv[1]);
    // printf("%s\n",argv[2]);

    char in_directory[4096];
    char out_directory[4096];

    parse_directory(in_directory, argv[1]);
    parse_directory(out_directory, argv[2]);

    FILE* infptr = fopen(in_directory, "r");

    FILE* outfptr = fopen(out_directory, "w");

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
