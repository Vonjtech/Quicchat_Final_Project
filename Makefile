CC=gcc
CFLAGS=-Wall -Icommon

all: server client

server: server/server.c common/protocol.h
	$(CC) $(CFLAGS) server/server.c -o quicchat_server

client: client/client.c common/protocol.h
	$(CC) $(CFLAGS) client/client.c -o quicchat_client

clean:
	rm -f quicchat_server quicchat_client
