CC=gcc
CFLAGS= -Wall -Wpedantic -Werror
EXEC_S=main.c
EXEC_E=main1

all: exec

exec:
	$(CC) $(EXEC_S) -o $(EXEC_E)
	./$(EXEC_E)
	make clean

clean:
	rm $(EXEC_E)