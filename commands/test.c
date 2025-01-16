#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void print_history() {
    // Get the history list
    HIST_ENTRY **history = history_list();
	printf("%d", history_length);
    if (history) {
        printf("\nCommand History:\n");
        for (int i = 0; history[i]; i++) {
            printf("%d: %s\n", i + 1, history[i]->line);
        }
    } else {
        printf("\nNo history available.\n");
    }
}

int main() {
    char *input;

    while ((input = readline("micro-shell> ")) != NULL) {
        if (*input) {
            add_history(input); // Add command to history
        }


        // Check if the user wants to see the history
        if (strcmp(input, "history") == 0) {
            print_history(); // Print the history
        } else {
            printf("You entered: %s\n", input);
        }

        free(input); // Free the allocated memory
    }

    return 0;
}
