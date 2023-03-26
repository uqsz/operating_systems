CC = gcc
CFLAGS= -Wall -Wpedantic -Werror

EXEC_SOURCES=main.c

EXIT_TARGET=main


all: exec

exec: 
	$(CC) $(CFLAGS) $(EXEC_SOURCES) -o $(EXIT_TARGET)

test: exec
	./$(EXIT_TARGET) sample_directory
	make clean

clean:
	rm -rf $(EXIT_TARGET)