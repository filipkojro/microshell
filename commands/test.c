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

int list_read(list* home_node, int position, char* value){
    list* current_node = home_node;

    for (int i = 0; i <= position; i++){
        if (current_node->next_node == NULL) return 1;
        current_node = current_node->next_node;
    }
    value = strdup(current_node->next_node->value);
    return 0;
}

int main() {
    list* arguments;

    list_create(&arguments);
    list_append(arguments, "first");
    list_append(arguments, "second");
    char* value;
    if (list_read(arguments, 0, value)) {
        printf("nie ma takiego");
    }
    else {
        printf("%s", value);
    }

    list_free(arguments);

    return 0;
}
