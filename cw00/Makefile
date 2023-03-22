CC=gcc
CFLAGS= -Wall -Wpedantic -Werror
EXEC_S=main.c
EXEC_E=main

all: exec

exec:
	$(CC) $(EXEC_S) -o $(EXEC_E)
	./main
	make clean

clean:
	rm $(EXEC_E)