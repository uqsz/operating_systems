CC=gcc
CFLAGS= -Wall -Wpedantic -Werror
EXEC_S1=catcher.c
EXEC_S2=sender.c
EXEC_E=main

all: exec

exec:
	touch temp.txt
	$(CC) $(CFLAGS) $(EXEC_S1) -o $(EXEC_E)_catcher
	$(CC) $(CFLAGS) $(EXEC_S2) -o $(EXEC_E)_sender


clean:
	rm main* temp.txt