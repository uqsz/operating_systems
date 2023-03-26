CC = gcc
CFLAGS= -Wall -Wpedantic -Werror

EXIT_TARGET = replace
EXEC_SOURCES = main.c

TEST_FILE_IN=in
TEST_FILE_OUT=out

FIND_CHAR=a
REVERSE_CHAR=B

RESULT_FILE=pomiar_zad_1

all: 
	make exec OPTION=1
	make exec OPTION=2

exec: 
	$(CC) $(CFLAGS) $(EXEC_SOURCES) -o $(EXIT_TARGET)_$(OPTION) -D OPTION=$(OPTION)

test: all
	touch $(RESULT_FILE).txt
	
	./$(EXIT_TARGET)_1 $(FIND_CHAR) $(REVERSE_CHAR) $(TEST_FILE_IN).txt $(TEST_FILE_OUT).txt >> $(RESULT_FILE).txt

	touch tmp1.txt

	tr $(FIND_CHAR) $(REVERSE_CHAR) < $(TEST_FILE_IN).txt > tmp1.txt
	diff -s tmp1.txt  $(TEST_FILE_OUT).txt >> $(RESULT_FILE).txt
	rm tmp1.txt

	./$(EXIT_TARGET)_2 $(FIND_CHAR) $(REVERSE_CHAR) $(TEST_FILE_IN).txt $(TEST_FILE_OUT).txt >> $(RESULT_FILE).txt

	touch tmp2.txt
	tr $(FIND_CHAR) $(REVERSE_CHAR) < $(TEST_FILE_IN).txt > tmp2.txt
	diff -s tmp2.txt  $(TEST_FILE_OUT).txt >> $(RESULT_FILE).txt
	echo " " >> $(RESULT_FILE).txt
	rm tmp2.txt

	make clean

clean:
	rm -rf $(EXIT_TARGET)_1 $(EXIT_TARGET)_2