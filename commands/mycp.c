#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

void parse_directory(char* output_directory, char* input_directory) {
    char* home_dir = getenv("HOME");

    if (input_directory != 0) {
        if (input_directory[0] == '~') {
            sprintf(output_directory, "%s%s", home_dir, input_directory + 1);
        } else {
            strcpy(output_directory, input_directory);
        }
    }
}

void copy_file(const char* src, const char* dst) {
    FILE* infptr = fopen(src, "r");
    if (infptr == NULL) {
        perror("Could not open file");
        return;
    }

    FILE* outfptr = fopen(dst, "w");
    if (outfptr == NULL) {
        perror("Could not open file");
        fclose(infptr);
        return;
    }

    char buffer[1024];
    int num;
    while ((num = fread(buffer, 1, sizeof(buffer), infptr)) > 0) {
        fwrite(buffer, 1, num, outfptr);
    }

    fclose(infptr);
    fclose(outfptr);
}

void copy_directory(const char* src, const char* dst) {
    struct stat st;
    if (stat(src, &st) != 0) {
        perror("stat");
        return;
    }

    if (S_ISDIR(st.st_mode)) {
        mkdir(dst, st.st_mode);

        DIR* dir = opendir(src);
        if (dir == NULL) {
            perror("opendir");
            return;
        }

        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }

            char src_path[4096];
            char dst_path[4096];
            snprintf(src_path, sizeof(src_path), "%s/%s", src, entry->d_name);
            snprintf(dst_path, sizeof(dst_path), "%s/%s", dst, entry->d_name);

            copy_directory(src_path, dst_path);
        }

        closedir(dir);
    } else if (S_ISREG(st.st_mode)) {
        copy_file(src, dst);
    } else {
        fprintf(stderr, "Unsupported file type: %s\n", src);
    }
}

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("mycp command needs at least 2 arguments\n");
        return 1;
    }

    char in_directory[4096];
    char out_directory[4096];

    parse_directory(in_directory, argv[1]);
    parse_directory(out_directory, argv[2]);

    copy_directory(in_directory, out_directory);

    return 0;
}