CC = gcc
CFLAGS= -Wall  -Werror

EXEC_SOURCES=main

EXIT_TARGET=main


all: exec_siginfo exec_resethand exec_nodefer

exec_siginfo: 
	$(CC) $(CFLAGS) $(EXEC_SOURCES)_siginfo.c -o $(EXIT_TARGET)_siginfo

exec_resethand: 
	$(CC) $(CFLAGS) $(EXEC_SOURCES)_resethand.c -o $(EXIT_TARGET)_resethand

exec_nodefer:
	$(CC) $(CFLAGS) $(EXEC_SOURCES)_nodefer.c -o $(EXIT_TARGET)_nodefer

test: all
	touch results.out
	./$(EXIT_TARGET)_siginfo > results.out
	make clean

clean:
	rm -rf $(EXIT_TARGET)_siginfo $(EXIT_TARGET)_resethand $(EXIT_TARGET)_nodefer