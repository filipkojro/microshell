#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define MAX_HISTORY 100
#define MAX_CMD_LEN 1024

char *history[MAX_HISTORY];
int history_count = 0;
int history_index = 0;

// Enable raw mode to capture keypresses
void enable_raw_mode() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Disable raw mode
void disable_raw_mode() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO); // Re-enable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Add command to history
void add_to_history(const char *cmd) {
    if (history_count < MAX_HISTORY) {
        history[history_count++] = strdup(cmd);
    }
    history_index = history_count; // Reset index
}

// Clear input
void clear_input(char* prompt, const char *cmd) {
    int term_width;
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {term_width = 80;}
    else {term_width = w.ws_col;}
    
    int cmd_lines = (strlen(cmd) + strlen(prompt)) / term_width + 1; // "+2" for "> " prompt
    for (int i = 0; i < cmd_lines; i++) {
        printf("\33[2K\r"); // Clear the current line
        if (i < cmd_lines - 1) {
            printf("\033[A"); // Move cursor up
        }
    }
    fflush(stdout);
    // printf("cleard %d lines", cmd_lines);
}

// Show a command on the current line
void show_command(char* prompt, const char *cmd) {
    // clear_input(cmd);
    printf("%s%s", prompt, cmd);
    fflush(stdout);
}

// Handle input with history navigation
void handle_input() {
    char cmd[MAX_CMD_LEN + 1] = {0};
    int cmd_len = 0;
    char c;

    char* prompt = "> ";

    enable_raw_mode();
    printf("%s",prompt);
    fflush(stdout);

    while (read(STDIN_FILENO, &c, 1) == 1) {
        if (c == '\n') { // Enter key
            cmd[cmd_len] = '\0';
            printf("\n");
            if (cmd_len > 0) {
                add_to_history(cmd);
                printf("You entered: %s\n", cmd);
            }
            cmd_len = 0;
            memset(cmd, 0, sizeof(cmd));
            // printf(">\n");
            write(STDOUT_FILENO, "> ", sizeof("> "));
            fflush(stdout);
        } else if (c == 27) { // Arrow keys
            char seq[2];
            if (read(STDIN_FILENO, &seq[0], 1) == 1 && read(STDIN_FILENO, &seq[1], 1) == 1) {
                if (seq[0] == '[') {
                    if (seq[1] == 'A') { // Up arrow
                        if (history_index > 0) {
                            history_index--;
                            clear_input(prompt, cmd);
                            show_command(prompt, history[history_index]);
                            strcpy(cmd, history[history_index]);
                            cmd_len = strlen(cmd);
                        }
                    } else if (seq[1] == 'B') { // Down arrow
                        if (history_index < history_count - 1) {
                            history_index++;
                            clear_input(prompt, cmd);
                            show_command(prompt, history[history_index]);
                            strcpy(cmd, history[history_index]);
                            cmd_len = strlen(cmd);
                        } else if (history_index == history_count - 1) {
                            history_index++;
                            clear_input(prompt, cmd);
                            memset(cmd, 0, sizeof(cmd));
                            write(STDOUT_FILENO, "> ", sizeof("> "));
                            fflush(stdout);
                            cmd_len = 0;
                        }
                    } else if (seq[1] == 'C') { // Right arrow
                        printf("Right?");
                        return;
                    } else if (seq[1] == 'D') { // Left arrow
                        printf("LEFT?");
                        return;
                    }
                }
            }
        } else if (c == 127) { // Backspace
            if (cmd_len > 0) {
                cmd[--cmd_len] = '\0';
                clear_input(prompt, cmd);
                show_command(prompt, cmd);
            }
        } else { // Regular characters
            if (cmd_len < MAX_CMD_LEN){
                cmd[cmd_len++] = c;
                write(STDOUT_FILENO, &c, 1);
            }
        }
    }

    disable_raw_mode();
}

int main() {
    handle_input();
    return 0;
}
