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

o:
	make all OPT_FLAGS="-O"

o0:
	make all OPT_FLAGS="-O0"

o1:
	make all OPT_FLAGS="-O1"

o2:
	make all OPT_FLAGS="-O2"

o3:
	make all OPT_FLAGS="-O3"

os:
	make all OPT_FLAGS="-Os"

exec_static: lib_static
	$(CC) $(CFLAGS) $(OPT_FLAGS) $(EXEC_SOURCES) $(LIB_TARGET).a -o $(EXEC_TARGET)_static

exec_shared: lib_shared
	$(CC) $(CFLAGS) $(OPT_FLAGS) $(EXEC_SOURCES) $(LIB_TARGET).so -o $(EXEC_TARGET)_shared

lib_static: $(SOURCES)
	$(CC) $(CFLAGS) $(OPT_FLAGS) -c $(LIB_SOURCES)
	ar rcs $(LIB_TARGET).a $(LIB_SOURCES:.c=.o)

lib_shared: $(SOURCES)
	$(CC) $(CFLAGS) $(OPT_FLAGS) -c -fPIC $(LIB_SOURCES)
	$(CC) $(CFLAGS) $(OPT_FLAGS) -shared -o $(LIB_TARGET).so $(LIB_SOURCES:.c=.o)

tests: exec_static exec_shared
	mkdir -p results
	./$(EXEC_TARGET)_static.exe < test.in > results/results_static_$(O_SUFFIX).txt 
	./$(EXEC_TARGET)_shared.exe < test.in > results/results_shared_$(O_SUFFIX).txt 

test_create:
	echo "init 200" >> test.in

	echo 'count libwc.h' >> test.in

	echo "show 0" >> test.in

	echo "delete 0" >> test.in

	echo "destroy" >> test.in 

	echo "exit" >> test.in

test: test_create
	make o
	make tests O_SUFFIX="O"
	make o0
	make tests O_SUFFIX="O0"
	make o1
	make tests O_SUFFIX="O1"
	make o2
	make tests O_SUFFIX="O2"
	make o3
	make tests O_SUFFIX="O3"
	make os
	make tests O_SUFFIX="Os"
	make o3
	make clean

clean:
	rm -rf *.o *.a *.so $(EXEC_TARGET)_static $(EXEC_TARGET)_shared *.in