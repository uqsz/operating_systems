CC=gcc
CFLAGS= -Wall -Wpedantic -Werror
EXEC_S=main
EXEC_E=main

all: exec

exec:
	$(CC) $(EXEC_S).c -o $(EXEC_E)
	$(CC) $(EXEC_S)1.c -o $(EXEC_E)1

clean:
	rm $(EXEC_E)