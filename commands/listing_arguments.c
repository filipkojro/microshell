#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list {
    char* value;
    struct list* next_node;
} list;

int list_create(list** home_node) {
    *home_node = (list*)malloc(sizeof(list));
    if (*home_node == NULL) return 1;
    (*home_node)->value = NULL;
    (*home_node)->next_node = NULL;
    return 0;
}

int list_append(list* home_node, char* value){
    if (strlen(value) >= 2048) return 1;

    list* current_node = home_node;

    while (current_node->next_node != NULL){
        current_node = current_node->next_node;
    }

    list* next_node = (list*)malloc(sizeof(list));
    if (next_node == NULL) return 1;
    
    next_node->value = strdup(value);
    if (next_node->value == NULL) return 1;

    next_node->next_node = NULL;
    current_node->next_node = next_node;

    return 0;
}

int list_free(list* home_node){
    list* current_node = home_node;
    list* next_node;

    while (current_node != NULL){
        next_node = current_node->next_node;
        free(current_node->value);
        free(current_node);
        current_node = next_node;
    }
    return 0;
}

int list_read(list* home_node, int position, char** value){
    list* current_node = home_node;

    for (int i = 0; i <= position; i++){
        if (current_node->next_node == NULL) return 1;
        current_node = current_node->next_node;
    }
    *value = current_node->value;
    return 0;
}

#define MAX_CMD_LEN 1024

int main() {
    list* arguments;

    list_create(&arguments);

    char* command =  "ls -0la nieccco akldnfa";

    char cmd[64] = {0};
    int cmd_len = strlen(command);
    memcpy(cmd, command, cmd_len);
    
    int some_counter = 0;
    char cur_cmd[MAX_CMD_LEN + 1] = {0};
    int argc = 0;

    for (int i = 0; i < cmd_len; i++){
        printf("%c\n", cmd[i]);
        if (cmd[i] == ' '){
            if (some_counter > 0){
                memcpy(cur_cmd, cmd + i - some_counter, some_counter);
                printf("zapis: %s\n\n", cur_cmd);
                list_append(arguments, cur_cmd);
                memset(cur_cmd, 0, some_counter);
                argc++;
            }
            
            some_counter = 0;
        }
        else {
            some_counter++;
        }
    }
    if (some_counter > 0){
        memcpy(cur_cmd, cmd + cmd_len - some_counter, some_counter);
        printf("zapis: %s\n\n", cur_cmd);
        list_append(arguments, cur_cmd);
        memset(cur_cmd, 0, some_counter);
        argc++;
    }

    char* argv[argc];

    char* value;
    for (int i = 0; i < argc; i++){
        if (list_read(arguments, i, &value)) {
        printf("nie ma takiego");
        }
        else {
            printf("lista[%d]=%s\n", i, value);
            argv[i] = strdup(value);
        }
    }
    list_free(arguments);
    

    for (int i = 0; i < argc; i++) printf("%s\n", argv[i]);


    return 0;
}
