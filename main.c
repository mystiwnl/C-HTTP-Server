#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <pthread.h>

void handle_client(int client_fd) {
    char buffer[1024] = {0};
    recv(client_fd, buffer, sizeof(buffer), 0);

    char* path = strtok(buffer, " ");
    if (path != NULL) {
        path = strtok(NULL, " ");
    }

    if (path == NULL || strcmp(path, "/") == 0) {
        char* response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nConnected to the server!\r\n";
        send(client_fd, response, strlen(response), 0);
    } else {
        if (path[0] == '/') {
            path++;
        }

        int opened_fd = open(path, O_RDONLY);
        if (opened_fd == -1) {
            char* response = "HTTP/1.1 404 Not Found\r\n\r\nFile not found!\r\n";
            send(client_fd, response, strlen(response), 0);
        } else {
            struct stat file_stat;
            fstat(opened_fd, &file_stat);
            off_t offset = 0;
            int sent_bytes = sendfile(client_fd, opened_fd, &offset, file_stat.st_size);
            if (sent_bytes == -1) {
                perror("sendfile");
            }
            close(opened_fd);
        }
    }
    close(client_fd);
}

int main() {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1) {
        perror("socket");
        return 1;
    }

    int opt = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(1441),
        .sin_addr.s_addr = INADDR_ANY,
    };

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind");
        return 1;
    }

    if (listen(s, 10) == -1) {
        perror("listen");
        return 1;
    }

    printf("Server listening on port 1441...\n");

    while (1) {
        int client_fd = accept(s, NULL, NULL);
        if (client_fd == -1) {
            perror("accept");
            continue;
        }


        pthread_t tid;
        if (pthread_create(&tid, NULL, (void *(*)(void *))handle_client, (void *)(intptr_t)client_fd) != 0) {
            perror("pthread_create");
            close(client_fd);
            continue;
        }
        pthread_detach(tid);
    }

    close(s);
    return 0;
}
