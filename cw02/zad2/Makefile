CC = gcc
CFLAGS= -Wall -Wpedantic -Werror

EXEC_SOURCES=main.c

RESULT_FILE=pomiar_zad_2

EXIT_TARGET=reverse

TEST_FILE_IN=in
TEST_FILE_OUT=out

all: 
	make exec BUFFER_SIZE=1
	make exec BUFFER_SIZE=1024

exec: 
	$(CC) $(CFLAGS) $(EXEC_SOURCES) -o $(EXIT_TARGET)_$(BUFFER_SIZE) -D BUFFER_SIZE=$(BUFFER_SIZE)

test: all
	touch $(RESULT_FILE).txt
	./$(EXIT_TARGET)_1 $(TEST_FILE_IN).txt $(TEST_FILE_OUT).txt >> $(RESULT_FILE).txt
	date >> $(TEST_FILE_OUT).txt
	./$(EXIT_TARGET)_1024 $(TEST_FILE_IN).txt $(TEST_FILE_OUT).txt >> $(RESULT_FILE).txt
	echo " " >> $(RESULT_FILE).txt
	make clean

clean:
	rm -rf *.exe