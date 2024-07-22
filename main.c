#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <stdlib.h>

void main() {

    int s = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr = {
        AF_INET,
        htons(1441),
        0
    };

    bind(s, (struct sockaddr *)&addr, sizeof(addr));

    listen(s, 10);

    while (1) {
        int client_fd = accept(s, 0, 0);
        char buffer[1024] = {0};
        recv(client_fd, buffer, sizeof(buffer), 0);


        char* path = strtok(buffer, " ");
        if (path != NULL) {
            path = strtok(NULL, " ");
        }

        if (path == NULL || strcmp(path, "/") == 0){
            char* response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nConnected to the server!\r\n";
            send(client_fd, response, strlen(response), 0);
        } else if (strncmp(path, "/hlfs.html", 14) == 0) {
            int opened_fd = open("hlfs.html", O_RDONLY);
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
        } else {
            char* response = "HTTP/1.1 404 Not Found\r\n\r\nFile not found!";
            send(client_fd, response, strlen(response), 0);
        }
        close(client_fd);
    }
}
