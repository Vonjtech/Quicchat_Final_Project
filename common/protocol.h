#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

#define MAX_PAYLOAD_SIZE 1024

typedef struct {
    uint8_t  version;
    uint8_t  type;
    uint16_t length;
    uint32_t session_id;
    uint32_t sender_id;
    uint32_t timestamp;
    uint8_t  payload[MAX_PAYLOAD_SIZE];
} __attribute__((packed)) cap_message_t;

enum cap_message_type {
    CAP_MSG_HANDSHAKE      = 0x01,
    CAP_MSG_AUTH_REQUEST   = 0x02,
    CAP_MSG_AUTH_SUCCESS   = 0x03,
    CAP_MSG_AUTH_FAILURE   = 0x04,
    CAP_MSG_TEXT           = 0x05,
    CAP_MSG_DELIVERY_ACK   = 0x06
};

#endif
