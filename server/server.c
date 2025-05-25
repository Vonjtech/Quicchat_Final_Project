#include "../common/protocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <time.h>

#define SERVER_PORT 12345
#define AUTH_USER "user"
#define AUTH_PASS "pass"

void handle_client(int client_fd) {
    cap_message_t msg;
    ssize_t bytes = read(client_fd, &msg, sizeof(msg));
    if (bytes <= 0) return;

    if (msg.type == CAP_MSG_HANDSHAKE) {
        printf("Handshake received\n");
    }

    bytes = read(client_fd, &msg, sizeof(msg));
    if (msg.type == CAP_MSG_AUTH_REQUEST) {
        char *received = (char *)msg.payload;
        if (strncmp(received, "user:pass", 9) == 0) {
            cap_message_t res = {1, CAP_MSG_AUTH_SUCCESS, sizeof(cap_message_t) - MAX_PAYLOAD_SIZE, 1, 0, time(NULL)};
            write(client_fd, &res, sizeof(res) - MAX_PAYLOAD_SIZE);
        } else {
            cap_message_t res = {1, CAP_MSG_AUTH_FAILURE, sizeof(cap_message_t) - MAX_PAYLOAD_SIZE, 1, 0, time(NULL)};
            write(client_fd, &res, sizeof(res) - MAX_PAYLOAD_SIZE);
            close(client_fd);
            return;
        }
    }

    while ((bytes = read(client_fd, &msg, sizeof(msg))) > 0) {
        if (msg.type == CAP_MSG_TEXT) {
            printf("Message from client: %s\n", msg.payload);
            cap_message_t ack = {1, CAP_MSG_DELIVERY_ACK, sizeof(cap_message_t) - MAX_PAYLOAD_SIZE, msg.session_id, msg.sender_id, time(NULL)};
            write(client_fd, &ack, sizeof(ack) - MAX_PAYLOAD_SIZE);
        }
    }

    close(client_fd);
}

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    listen(sockfd, 5);
    printf("Server listening on port %d...\n", SERVER_PORT);

    while (1) {
        int client_fd = accept(sockfd, NULL, NULL);
        if (client_fd >= 0) {
            if (!fork()) {
                close(sockfd);
                handle_client(client_fd);
                exit(0);
            }
            close(client_fd);
        }
    }
}
