CC=gcc
CFLAGS= -Wall -Wpedantic -Werror
EXEC_S1=server.c
EXEC_S2=client.c
EXEC_E=main

all: exec

exec:
	rm console_logs.txt
	touch console_logs.txt
	echo '' > console_logs.txt
	$(CC) $(CFLAGS) $(EXEC_S1) -o server
	$(CC) $(CFLAGS) $(EXEC_S2) -o client


clean:
	rm server client 