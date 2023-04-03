CC = gcc
CFLAGS= -Wall -Wpedantic -Werror

EXEC_SOURCE=main.c

EXIT_TARGET1=main_fork
EXIT_TARGET2=main_exec
EXIT_TARGET3=main_simple


all: exec_fork exec_exec exec_clear

exec_fork: 
	$(CC) $(CFLAGS) $(EXEC_SOURCE) -o $(EXIT_TARGET1) -D FORK=0

exec_exec:
	$(CC) $(CFLAGS) $(EXEC_SOURCE) -o $(EXIT_TARGET2) -D EXEC=0

exec_clear:
	$(CC) $(CFLAGS) $(EXEC_SOURCE) -o $(EXIT_TARGET3)

test_fork: exec_fork
	touch results.out
	echo ":::FORK TEST:::\n" > results.out
	./$(EXIT_TARGET1) ignore >> results.out
	echo "" >> results.out
	./$(EXIT_TARGET1) handler >> results.out
	echo "" >> results.out
	./$(EXIT_TARGET1) mask >> results.out
	echo "" >> results.out
	./$(EXIT_TARGET1) pending >> results.out
	echo "\n\n" >> results.out
	

test_exec: exec_exec exec_clear
	touch results.out
	echo ":::EXEC TEST:::\n" >> results.out
	./$(EXIT_TARGET2) ignore >>results.out
	echo "" >> results.out
	./$(EXIT_TARGET2) mask >> results.out
	echo "" >> results.out
	./$(EXIT_TARGET2) pending >> results.out
	make clean

test: test_fork test_exec


clean:
	rm -rf $(EXIT_TARGET1) $(EXIT_TARGET2) $(EXIT_TARGET3)