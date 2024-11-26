#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <stdio.h>

int main() {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir("/")) == NULL) perror("Error opendir() function");
    else {
        printf("Content of / directory:\n");

        while ((entry = readdir(dir)) != NULL)
            printf("  %s\n", entry->d_name);

        closedir(dir);
    }

    return 0;
}