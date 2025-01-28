#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <termios.h>
#include <sys/ioctl.h>


// colors using ANSI escape code \033[1;{color code}m
#define BLINK "\001\033[5m\002"
#define RED_TEXT "\001\033[1;31m\002"
#define GREEN_TEXT "\001\033[1;32m\002"
#define BLUE_TEXT "\001\033[1;34m\002"
#define RESET_TEXT "\001\033[1;0m\002"

#define MAX_HISTORY 100
#define MAX_CMD_LEN 1024

char *history[MAX_HISTORY];
int history_count = 0;
int history_index = 0;

char* concat(const char *s1, const char *s2) {
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1); // +1 for terminator
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1); // +1 to copy terminator
    return result;
}

// Insert char inside string on position
void insert_inside(char* cmd, int* pos, int* len, char c){
    char* temp = malloc(*len);
    memcpy(temp, cmd, *len);
    cmd[*pos] = c;
    memcpy(cmd + *pos + 1, temp + *pos, *len - *pos);

    (*pos)++;
    (*len)++;
    free(temp);
}

// Delete char inside string on position
void delete_inside(char* cmd, int* pos, int* len){
    char* temp = malloc(*len);
    memcpy(temp, cmd, *len);
    memcpy(cmd + *pos - 1, temp + *pos, *len - *pos);

    (*pos)--;
    (*len)--;
    free(temp);
    cmd[*len] = '\0';
}

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

int terminal_width(){
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {return 80;}
    else {return w.ws_col;}
}

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
void clear_input(int prompt_len, const char *cmd) {
    int cmd_lines = (strlen(cmd) + prompt_len - 1) / terminal_width() + 1; // "+2" for "> " prompt
    for (int i = 0; i < cmd_lines; i++) {
        printf("\001\33[2K\r\002"); // Clear the current line
        if (i < cmd_lines - 1) {
            printf("\001\033[A\002"); // Move cursor up
        }
    }
    fflush(stdout);
}

// Show a command on the current line
void show_command(char* prompt, const char *cmd) {
    // clear_input(cmd);
    printf("%s%s", prompt, cmd);
    fflush(stdout);
}

// Move terminal cursor to the correct position
void move_cursor(const char* cmd, int prompt_len, int* pos, int move){
    printf("\001\033[%dG\002", (prompt_len + *pos + 1) % terminal_width());
    fflush(stdout);
}

int gen_prompt(char* prompt){
    char whole_cwd[4096];
    char cwd[4096];
    char* user = getlogin();
    if (user == NULL) user = "";
    // user = getlogin();
    // char user[1024];
    
    char hostname[1024];

    // if (getlogin_r(user, sizeof(user)) != 0){
    //     printf(RED_TEXT"getlogin() error?\n");
    //     return 1;
    // }

    // if (gethostname(hostname, sizeof(hostname)) != 0){
    //     printf(RED_TEXT"gethostname() error?\n");
    //     return 1;
    // }

    if (getcwd(whole_cwd, sizeof(whole_cwd)) == NULL) {
        printf(RED_TEXT"getcwd() error?\n");
        return 1;
    }

    char* home_dir;
    if ((home_dir = getenv("HOME")) == NULL){
        printf(RED_TEXT"HOME error?\n");
        return 1;
    }

    char possible_home[4096];

    if (strlen(home_dir) <= strlen(whole_cwd)){
        strncpy(possible_home, whole_cwd, strlen(home_dir));
        possible_home[strlen(home_dir)] = '\0';

        if (strcmp(possible_home, home_dir) == 0){
            snprintf(cwd, sizeof(cwd), "~%s", whole_cwd + strlen(home_dir));
            // strcpy(cwd, concat("~", whole_cwd + strlen(home_dir)));
        }
        else {
            strncpy(cwd, whole_cwd, sizeof(cwd) - 1);
            cwd[sizeof(cwd) - 1] = '\0'; // Null-terminate
            // strcpy(cwd, whole_cwd);
        }
    }
    else {
        strncpy(cwd, whole_cwd, sizeof(cwd) - 1);
        cwd[sizeof(cwd) - 1] = '\0'; // Null-terminate
        // strcpy(cwd, whole_cwd);
    }

    snprintf(prompt, 4096, RESET_TEXT"!microshell!"BLUE_TEXT"%s@%s "GREEN_TEXT"%s"RESET_TEXT" $ ", user, hostname, cwd);

    

    int vis_propmt = (int)(strlen(user) + strlen(hostname) + strlen(cwd) + strlen("!microshell!@  $ "));

    return vis_propmt;
}

void handle_sigint(int sig) {
    printf("^C\n");
    return;
}

int mycd(int argc, char **argv){
    printf("%s\n", argv[0]);
    printf("%s\n", argv[1]);

    char* home_dir = getenv("HOME");
    char directory[4096];

    if (argc != 0){
        if (argv[1][0] == '~'){
            sprintf(directory, "%s%s", home_dir, argv[1] + 1);
        }
        else {
            strcpy(directory, argv[1]);
        }
    }
    // printf("next dir:%s", directory);
    if (chdir(directory) != 0){
        printf("problem with chdir\n");
        return 1;
    }
    return 0;
}

int main(){
    signal(SIGINT, handle_sigint);

    char cwd[4096];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        printf(RED_TEXT"getcwd() error?\n");
        return 1;
    }
    // Addming microshell commands to PATH
    setenv("PATH", concat(concat(cwd, "/commands/bin:"), getenv("PATH")), 1);

    char prompt[8192];
    int prompt_len;
    if((prompt_len = gen_prompt(prompt)) == 1){
        exit(1);
    }

    char cmd[MAX_CMD_LEN + 1] = {0};
    int cmd_len = 0;
    int cmd_cur = 0;
    char c;

    enable_raw_mode();
    printf("%s",prompt);
    fflush(stdout);

    while (read(STDIN_FILENO, &c, 1) == 1) {
        if (c == '\n') { // Enter key
            cmd[cmd_len] = '\0';
            printf("\n");


            // Code logic after everyting is inserted
            if (cmd_len > 0) {
                add_to_history(cmd);

                // printf("You entered: %s\n", cmd);
            

                // add_history(inp);

                
                list* arguments;

                list_create(&arguments);
                
                int argument_coutner = 0;
                char cur_cmd[MAX_CMD_LEN + 1] = {0};
                int argc = 0;

                for (int i = 0; i < cmd_len; i++){
                    if (cmd[i] == ' '){
                        if (argument_coutner > 0){
                            memcpy(cur_cmd, cmd + i - argument_coutner, argument_coutner);
                            list_append(arguments, cur_cmd);
                            memset(cur_cmd, 0, argument_coutner);
                            argc++;
                        }
                        
                        argument_coutner = 0;
                    }
                    else {
                        argument_coutner++;
                    }
                }
                if (argument_coutner > 0){
                    memcpy(cur_cmd, cmd + cmd_len - argument_coutner, argument_coutner);
                    list_append(arguments, cur_cmd);
                    memset(cur_cmd, 0, argument_coutner);
                    argc++;
                }

                char* argv[argc + 1];

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
                
                // Creating argv
                for (int i = 0; i < argc; i++) printf("argv:%s\n", argv[i]);
                argv[argc] = NULL;

                if (strcmp(argv[0], "exit") == 0){
                    printf("leaving :)\n");
                    signal(SIGINT, SIG_DFL);
                    disable_raw_mode();
                    exit(0);
                }
                else if (strcmp(argv[0], "mycd") == 0){
                    if (mycd(argc, argv) != 0){
                        printf("proglem with mycd\n");
                    }
                }
                else {
                    int childpid;

                    if ((childpid = fork()) != 0){
                        // printf("rodzic?\n");

                        int status;
                        wait(&status);

                        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                            printf(RED_TEXT"\aError number: %d\n"RESET_TEXT, WEXITSTATUS(status));
                        }
                    }
                    else {
                        signal(SIGINT, SIG_DFL);

                        execvp(argv[0], argv);
                        printf(RED_TEXT"nie ma takiej komendy\n");
                        _exit(errno);

                        return 0;
                    }
                }
                

                if((prompt_len = gen_prompt(prompt)) == 1){
                    exit(1);
                }
            }



            cmd_len = 0;
            cmd_cur = cmd_len;
            memset(cmd, 0, sizeof(cmd));
            // printf(">\n");
            write(STDOUT_FILENO, prompt, strlen(prompt));
            fflush(stdout);
        } else if (c == 27) { // Arrow keys
            char seq[2];
            if (read(STDIN_FILENO, &seq[0], 1) == 1 && read(STDIN_FILENO, &seq[1], 1) == 1) {
                if (seq[0] == '[') {
                    if (seq[1] == 'A') { // Up arrow
                        if (history_index > 0) {
                            history_index--;
                            clear_input(prompt_len, cmd);
                            show_command(prompt, history[history_index]);
                            strcpy(cmd, history[history_index]);
                            cmd_len = strlen(cmd);
                            cmd_cur = cmd_len;
                        }
                    } else if (seq[1] == 'B') { // Down arrow
                        if (history_index < history_count - 1) {
                            history_index++;
                            clear_input(prompt_len, cmd);
                            show_command(prompt, history[history_index]);
                            strcpy(cmd, history[history_index]);
                            cmd_len = strlen(cmd);
                            cmd_cur = cmd_len;
                        } else if (history_index == history_count - 1) {
                            history_index++;
                            clear_input(prompt_len, cmd);
                            memset(cmd, 0, sizeof(cmd));
                            write(STDOUT_FILENO, prompt, strlen(prompt));
                            fflush(stdout);
                            cmd_len = 0;
                            cmd_cur = cmd_len;
                        }
                    } else if (seq[1] == 'C') { // Right arrow
                        if (cmd_cur < cmd_len) {
                            cmd_cur++; // Move logical cursor right
                            clear_input(prompt_len, cmd);
                            show_command(prompt, cmd);

                            // Move terminal cursor
                            move_cursor(cmd, prompt_len, &cmd_cur, 1);
                        }
                    } else if (seq[1] == 'D') { // Left arrow
                        if (cmd_cur > 0) {
                            cmd_cur--; // Move logical cursor left
                            clear_input(prompt_len, cmd);
                            show_command(prompt, cmd);

                            
                            move_cursor(cmd, prompt_len, &cmd_cur, -1);
                        }
                    }
                }
            }
        } else if (c == 127) { // Backspace
            if (cmd_len > 0 && cmd_cur > 0) {
                clear_input(prompt_len, cmd);
                delete_inside(cmd, &cmd_cur, &cmd_len);
                show_command(prompt, cmd);
                move_cursor(cmd, prompt_len, &cmd_cur, -1);
            }
        } else { // Regular characters
            if (cmd_len < MAX_CMD_LEN){
                clear_input(prompt_len, cmd);
                insert_inside(cmd, &cmd_cur, &cmd_len, c);
                show_command(prompt, cmd);
                move_cursor(cmd, prompt_len, &cmd_cur, 1);
            }
        }
    }

    disable_raw_mode();

    return 0;
}
