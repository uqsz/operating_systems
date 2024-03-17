# Operating Systems

This repository contains a series of labs related to operating systems concepts.

## Contents:

1. **[Lab1 - Memory management and libraries](#memory-management-and-libraries)**
2. **[Lab2 - Files](#files)**
3. **[Lab3 - Processes](#processes)**
4. **[Lab4 - Signals](#signals)**
5. **[Lab5 - Pipes](#pipes)**
6. **[Lab6 - Inter Process Communication (IPC)](#inter-process-communication)**

## Memory management and libraries

Design and prepare a set of functions (library) to manage an array of blocks where results of line, word, and character counting operations (using the wc command) in files passed as appropriate parameters are stored.

## Files

#### Exercise 1.

A program that takes 4 command-line arguments:

- ASCII character to be found in the input file,
- ASCII character to which all occurrences of the first argument should be replaced,
- Input file name in which to find the first argument,
- Output file name to which to save the content of the input file with replaced characters argv[1] to argv[2],

and compares the output with the result of the `tr` command:

```bash
./replace [char1] [char2] input_file output_file ; tr [char1] [char2] < input_file > tmp ; diff -s tmp output_file
```

#### Exercise 2.

Write a program that copies the contents of one file to another, reversing byte by byte.

## Processes

#### Exercise 1.

Program that accepts one argument: argv[1]. The program creates argv[1] child processes. Each child process prints two identifiers on standard output in one line: the identifier of the parent process and its own identifier. At the end of the standard output, the parent process prints argv[1].

#### Exercise 2.

Program that accepts one argument: argv[1], which is the directory path. The program prints its own name without a newline character using printf, and then executes the /bin/ls program with the directory path using execl.

#### Exercise 3.

Program that accepts two arguments: argv[1], which is the directory path, and argv[2], which is a string of characters no longer than 255 bytes. The program recursively traverses the directory argv[1] and its subtree of subdirectories, creating a process tree that mirrors the directory tree, with one process for each directory. Each process traverses all the files in the directory.

## Signals

#### Exercise 1.

Program demonstrates whether signal dispositions, their masks, and pending signals are inherited after calling fork and exec functions.

#### Exercise 2.

Testing the behavior of three selected flags in the sigaction function. One of them should be the SA_SIGINFO flag.

#### Exercise 3.

Two programs: sender program sending signals SIGUSR1 and catcher program receiving these signals. Along with each signal sent by the sender, the operating mode of the catcher program should be sent using the sigqueue function. Possible operating modes:

- print numbers from 1 to 100 on standard output
- print current time on standard output
- print the number of received requests to change the operating mode since the beginning of the program
- print current time every second until receiving a different operating mode
- terminate the catcher program.

The catcher retrieves the PID of the sender from the siginfo structure.

## Pipes

#### Exercise 1.

Program that numerically calculates the definite integral value of the function 4/(x^2+1) in the interval from 0 to 1 using the rectangle method (Riemann sum). The first parameter of the program is the width of each rectangle, determining the accuracy of the calculations. The calculations are be divided into n child processes (the second parameter of the program call), so that each process calculates a different part of the interval defined above.

#### Exercise 2.

Program that numerically calculates the definite integral value as in exercise 1, but the calculations are divided not into several processes, but into several programs.

## Inter Process Communication

Simple client-server program where communication is realized using message queues. After starting, the server creates a new System V message queue. Clients will send messages to the server using this queue. The sent requests contains the type of request as the type of message and information about which client they were sent from (client ID). In response, the type of message should be information identifying the waiting client.

The client, immediately after starting, creates a queue with a unique IPC key and sends its key to the server as a message (INIT message). Upon receiving such a message, the server opens the client's queue, assigns an identifier to the client (e.g., a number in the order of applications), and sends this identifier back to the client (communication from server to client is done via the client's queue). After receiving the identifier, the client can send a request to the server (requests are read from the standard output in the form of message_type).

Types of requests:

- LIST: Order to print a list of all active clients.
- 2ALL string: Order to send a message to all other clients. The client sends a string of characters. The server sends this string along with the sender client ID and the current date to all other clients.
- 2ONE client_id string: Order to send a message to a specific client. The client sends a string of characters specifying the recipient client by their ID from the list of active clients. The server sends this string along with the sender client ID and the current date to the specified client.
- STOP: Notification of the client's work termination. The client sends this message when it finishes work so that the server can remove its queue from the list. Then, it terminates its work by deleting its queue. This message is also sent when the client receives a SIGINT signal.

The server never writes to a file the time of receiving the request, client ID, and the content of the message. Requests are handled according to priorities, with STOP having the highest priority, followed by LIST and the rest.
