#include <stdio.h>
#include <string.h>

typedef struct list_node {
    char* argument;
    struct list_node* next_argument;
} list_node;

int main() {
    char cmd[64] = {0};
    char* command = "hello world nice  not";
    int cmd_len = strlen(command);
    memcpy(cmd, command, strlen(command));

    list_node args;
    list_node next_arg;

    int counter = 0;
    for (int i = 0; i < cmd_len; i++){
        if (cmd[i] == ' '){
            
            counter = 0;
        }
        else {
            counter++;
        }
    }

    printf("%s", cmd);

    return 0;
}
