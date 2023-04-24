CC=gcc
CFLAGS= -Wall -Wpedantic -Werror
EXEC_S1=server.c
EXEC_S2=client.c
EXEC_E=main

all: exec

exec:
	$(CC) $(CFLAGS) $(EXEC_S1) -o server
	$(CC) $(CFLAGS) $(EXEC_S2) -o client


clean:
	rm server client 