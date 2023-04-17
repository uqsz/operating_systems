CC=gcc
CFLAGS= -Wall -Wpedantic -Werror
EXEC_S1=main.c
EXEC_S2=calc.c
EXIT_TARGET1=main
EXIT_TARGET2=calc


all: exec

exec:
	$(CC) $(CFLAGS) $(EXEC_S1) -o $(EXIT_TARGET1)
	$(CC) $(CFLAGS) $(EXEC_S2) -o $(EXIT_TARGET2)

test: exec
	touch results.txt
	./$(EXIT_TARGET1) 0.0000001 5 > results.txt
	./$(EXIT_TARGET1) 0.00000001 10 >> results.txt
	./$(EXIT_TARGET1) 0.000000001 15 >> results.txt
	./$(EXIT_TARGET1) 0.000000001 20 >> results.txt
	./$(EXIT_TARGET1) 0.000000001 25 >> results.txt
	make clean

clean:
	rm main calc