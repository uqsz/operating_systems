CC=gcc
CFLAGS= -Wall -Wpedantic -Werror
EXEC_S=main
EXEC_E=main
EXEC_S1=main1
EXEC_E1=main1

all: exec

exec:
	$(CC) $(EXEC_S).c -o $(EXEC_E)
	$(CC) $(EXEC_S1).c -o $(EXEC_E1)

clean:
	rm $(EXEC_E) $(EXEC_E1)