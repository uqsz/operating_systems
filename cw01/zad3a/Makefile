CC = gcc
CFLAGS= -Wall -Wpedantic -Werror
SOURCES = $(wildcard *.c)

LIB_TARGET = libwc
LIB_SOURCES = libwc.c

EXIT_TARGET = main
EXEC_SOURCES = main.c
EXEC_TARGET = main

all: exec_static exec_shared
.PHONY: clean

exec_static: lib_static
	$(CC) $(CFLAGS) $(EXEC_SOURCES) $(LIB_TARGET).a -o $(EXEC_TARGET)_static

exec_shared: lib_shared
	$(CC) $(CFLAGS) $(EXEC_SOURCES) $(LIB_TARGET).so -o $(EXEC_TARGET)_shared

lib_static: $(SOURCES)
	$(CC) $(CFLAGS) -c $(LIB_SOURCES)
	ar rcs $(LIB_TARGET).a $(LIB_SOURCES:.c=.o)

lib_shared: $(SOURCES)
	$(CC) $(CFLAGS) -c -fPIC $(LIB_SOURCES)
	$(CC) $(CFLAGS) -shared -o $(LIB_TARGET).so $(LIB_SOURCES:.c=.o)

test: exec_static exec_shared
	make test_create
	touch results_shared.txt
	touch results_static.txt
	./$(EXEC_TARGET)_static < test.in > results_static.txt 

	make clean
test_create:
	echo "init 200" > test.in

	(ls -p | grep -E "\.(c|h)" | sed 's/^/count /') >> test.in

	echo "show 0" >> test.in
	echo "show 1" >> test.in
	echo "show 2" >> test.in

	echo "delete 0" >> test.in
	echo "delete 1" >> test.in
	echo "delete 2" >> test.in

	echo "destroy" >> test.in 

	echo "exit" >> test.in

clean:
	rm -rf *.o *.a *.so $(EXEC_TARGET)_static $(EXEC_TARGET)_shared *.in