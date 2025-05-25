# QuicChat Protocol – CS544 Final Project

**Author:** Ibrahim Vonjoe  
**Course:** CS544 – Computer Networks  
**Instructor:** Prof. Brian Mitchell  
**Submission:** Term Project – Part 3: Network Protocol Implementation

---

## Project Summary

**QuicChat** is a stateful chat protocol implemented over TCP using a custom binary format in C. It simulates a lightweight, extensible communication system that validates core principles of protocol design, including message framing, state transitions, authentication, and delivery guarantees.

This implementation is based on the protocol specification submitted in Part 2 of the CS544 project and includes a functioning client-server model with the essential protocol logic and a compliant finite state machine.

---

## Features Implemented

- *Custom Binary Protocol Structure** (`cap_message_t`)
- **User Authentication** with fixed credentials (`user:pass`)
- **DFA-compliant State Machine**
  - S0: Connection → Handshake
  - S1: Auth Request → Auth Result
  - S2: Message Exchange
- **Private Messaging** with structured payloads
- **Delivery Acknowledgment** from server to client
- **Timestamps** included in all messages
- **Minimal CLI Interface** for portability
- **Extensible Message Type System**

---

## Protocol Specification

All messages use the following packed binary format:

```c
typedef struct {
    uint8_t  version;
    uint8_t  type;
    uint16_t length;
    uint32_t session_id;
    uint32_t sender_id;
    uint32_t timestamp;
    uint8_t  payload[MAX_PAYLOAD_SIZE];
} __attribute__((packed)) cap_message_t;
```

### Supported Message Types

| Code | Message Type         | Description                       |
|------|----------------------|-----------------------------------|
| 0x01 | CAP_MSG_HANDSHAKE    | Initial connection negotiation    |
| 0x02 | CAP_MSG_AUTH_REQUEST | Credentials sent to server        |
| 0x03 | CAP_MSG_AUTH_SUCCESS | Login accepted                    |
| 0x04 | CAP_MSG_AUTH_FAILURE | Login rejected                    |
| 0x05 | CAP_MSG_TEXT         | Plain text message                |
| 0x06 | CAP_MSG_DELIVERY_ACK| Acknowledgment of message receipt |

*See `common/protocol.h` for all constants.*

---

## Build Instructions

Run on Linux system or WSL (Windows Subsystem for Linux). Then run:

```bash
make
```

This compiles both:
- `quicchat_server`
- `quicchat_client`

To clean and rebuild:

```bash
make clean
make
```

---

## Running the Application

### Start the Server (Terminal 1)
```bash
./quicchat_server
```

Expected output:
```
Server listening on port 12345...
```

### Start the Client (Terminal 2)
```bash
./quicchat_client 127.0.0.1 user:pass
```

Expected output:
```
Authentication successful.
Enter message:
```

You can now begin typing and sending messages to the server.

---

## Sample Interaction

```bash
> ./quicchat_client 127.0.0.1 user:pass
Authentication successful.
Enter message: Hello, QuicChat!
```

Server terminal:
```
Server listening on port 12345...
Handshake received
Received message type from client: 0x02
Authentication request validated.
Message from client: Hello, QuicChat!
```

---

## Project Structure

```
quicchat_final_project/
├── client/               # Client-side logic
│   └── client.c
├── server/               # Server-side logic
│   └── server.c
├── common/               # Shared message and protocol definitions
│   └── protocol.h
├── Makefile              # Build system
└── README.md             # Project documentation
```

---

## DFA Compliance

| State | Trigger              | Transition | Implemented |
|-------|----------------------|------------|-------------|
| S0    | TCP Connect          | → S1       | Yes       |
| S1    | AUTH Request (valid) | → S2       | Yes       |
| S1    | AUTH Request (fail)  | → S0 (exit)| Yes       |
| S2    | Send/Recv Text       | Loop       | Yes       |
| S2    | Close connection     | → S0       | Yes       |

---

## Security & Design Notes

- Authentication is handled via a simple string check (`user:pass`)
- Protocol includes version and timestamp fields for extensibility and replay detection
- Server sends structured ACKs for every valid message
- Ready to support QUIC/TLS if transport layer upgraded

---

## Future Enhancements (Deferred for Scope)

- Group messaging and broadcast support
- Message history (`CAP_MSG_HISTORY_REQ/RESP`)
- User presence and status indicators
- QUIC transport integration
- Persistent user storage or OAuth2 integration

---

## Conclusion

This implementation validates the design presented in Part 2 and demonstrates hands-on mastery of:
- Protocol design
- Socket programming in C
- Stateful server-client logic
- Binary message serialization
- Terminal-based chat systems

---

## Author

**Ibrahim Vonjoe**  
Student, CS544 – Computer Networks  
Email: *iev24@drexel.edu*  
GitHub: *(https://github.com/Vonjtech/Quicchat_Final_Project)*
# Quicchat_Final_Project
