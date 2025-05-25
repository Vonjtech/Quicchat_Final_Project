#include "../common/protocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>

#define SERVER_PORT 12345

void send_message(int sock, uint8_t type, const char *payload, uint32_t session_id) {
    cap_message_t msg = {
        .version = 1,
        .type = type,
        .length = sizeof(cap_message_t) - MAX_PAYLOAD_SIZE + strlen(payload),
        .session_id = session_id,
        .sender_id = 1,
        .timestamp = time(NULL)
    };
    strcpy((char *)msg.payload, payload);
    write(sock, &msg, sizeof(msg) - MAX_PAYLOAD_SIZE + strlen(payload));
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <server_ip> <username:password>\n", argv[0]);
        return 1;
    }

    printf("Connecting to server at %s:%d...\n", argv[1], SERVER_PORT);

    struct hostent *server = gethostbyname(argv[1]);
    if (!server) {
        fprintf(stderr, "Error: no such host %s\n", argv[1]);
        return 1;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Error creating socket");
        return 1;
    }

    struct sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    memcpy(&serv_addr.sin_addr, server->h_addr_list[0], server->h_length);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error connecting to server");
        close(sock);
        return 1;
    }

    send_message(sock, CAP_MSG_HANDSHAKE, "", 1);
    sleep(1); // Wait for server to process handshake
    send_message(sock, CAP_MSG_AUTH_REQUEST, argv[2], 1);

    cap_message_t response;
    ssize_t bytes = read(sock, &response, sizeof(response));
    if (bytes <= 0) {
        printf("Error: no response from server. Is it still running?\n");
        close(sock);
        return 1;
    }

    printf("Received message type: 0x%02X (%d bytes)\n", response.type, (int)bytes);

    if (response.type == CAP_MSG_AUTH_SUCCESS) {
        printf("Authentication successful.\n");
    } else if (response.type == CAP_MSG_AUTH_FAILURE) {
        printf("Authentication failed.\n");
        close(sock);
        return 1;
    } else {
        printf("Unexpected response from server.\n");
    }

    char input[256];
    while (1) {
        printf("Enter message: ");
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = 0;
        if (strlen(input) == 0) continue;  // Skip empty lines
        send_message(sock, CAP_MSG_TEXT, input, 1);
    }

    close(sock);
    return 0;
}