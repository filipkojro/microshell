#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080 // Define the port for the HTTP server
#define RESPONSE_START "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"

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

// Function to handle client requests
void handle_client(int client_socket, char* http_path) {
    char buffer[1024];
    int bytes_read;

    // Read the client's request
    bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read client request");
        close(client_socket);
        return;
    }
    buffer[bytes_read] = '\0'; // Null-terminate the request

    // Print the request to the console (for debugging)
    printf("Received request:\n%s\n", buffer);

    // Send a simple HTTP response
    write(client_socket, RESPONSE_START, strlen(RESPONSE_START));

    char directory[4096];

    parse_directory(directory, http_path);

    char dir_buffer[1024];

    FILE* fptr = fopen(directory, "r");
    if (fptr == NULL) {
        perror("Failed to open file");
        close(client_socket);
        return;
    }
    int num;
    while ((num = fread(dir_buffer, 1, sizeof(dir_buffer), fptr)) > 0) {
        write(client_socket, buffer, num);
    }

    fclose(fptr);

    // Close the connection to the client
    close(client_socket);
}

int main(int argc, char **argv) {
    int server_port = atoi(argv[1]);
    char* server_html_path = argv[2];

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Create the server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Failed to create socket");
        return 1;
    }

    // Configure the server address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(server_port);

    // Bind the socket to the specified port
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to bind to port");
        close(server_socket);
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_socket, 10) < 0) {
        perror("Failed to listen for connections");
        close(server_socket);
        return 1;
    }

    printf("HTTP server is running on port %d\n", server_port);

    // Main loop to accept and handle incoming connections
    while (1) {
        // Accept an incoming connection
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            perror("Failed to accept connection");
            continue;
        }

        // Handle the client's request in a separate function
        handle_client(client_socket, server_html_path);
    }

    // Close the server socket
    close(server_socket);
    return 0;
}