#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define RED_TEXT "\033[1;31m"

char* concat(const char *s1, const char *s2) {
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1); // +1 for terminator
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1); // +1 to copy terminator
    return result;
}

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

int main(int argc, char **argv) {
    DIR *dir;
    struct dirent *entry;

    char directory[4096];
    if (getcwd(directory, sizeof(directory)) == NULL) {
        printf(RED_TEXT"getcwd() error?\n");
        return 1;
    }
    int as_list_flag = 0;
    int all_flag = 0;
    // printf("%s\n", directory);

    // handling arguments

    for (int i = 1; i < argc; i++){
        // printf("%d:%s\n", i, argv[i]);

        if (strcmp(argv[i], "-l") == 0) {
            as_list_flag = 1;
            continue;
        }
        if (strcmp(argv[i], "-a") == 0) {
            all_flag = 1;
            continue;
        }
        parse_directory(directory, argv[i]);
        // strcpy(directory, argv[i]);
    }

    // printf("what is my directory: %s\n\n\n", directory);

    char next_directory[1024];

    if ((dir = opendir(directory)) == NULL) perror("Error opendir() function");
    else {
        // printf("Content of %s directory:\n", directory);

        while ((entry = readdir(dir)) != NULL) {
            strcpy(next_directory, entry->d_name);

            if (next_directory[0] != '.' || all_flag == 1){
                if (as_list_flag == 0){
                    printf("%s\t", next_directory);
                }
                if (as_list_flag == 1){
                    printf("%s\n", next_directory);
                }
            }
        }
        closedir(dir);
        if (as_list_flag == 0) printf("\n");
    }

    return 0;
}