#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char* argv[])
{
    int port;
    int server_fd;
    int client_fd;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t addr_len;
    char buffer[BUFFER_SIZE];
    ssize_t num_bytes;

    port = atoi(argv[2]);

    addr_len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))
        < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(1);
    }

    if (listen(server_fd, 1) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(1);
    }
    printf("Echo server is listening on port %d\n", port);

    while (1) {
        client_fd
            = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
        printf("Client connected\n");

        while ((num_bytes = recv(client_fd, buffer, BUFFER_SIZE - 1, 0)) > 0) {
            buffer[num_bytes] = '\0';
            printf("Received: %s", buffer);
            send(client_fd, buffer, num_bytes, 0);
        }

        printf("Client disconnected\n");
        close(client_fd);
    }

    close(server_fd);

    return 0;
}
