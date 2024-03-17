#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 128

pid_t read_pid(){
    pid_t pid;
    FILE *file =fopen("temp.txt","r");
    if (file==NULL){
        perror("opening temp");
        exit(EXIT_FAILURE);
    }
    fscanf(file,"%d", &pid);
    fclose(file);
    return pid;
}

void handler(int sig){
    printf("The signal has been delivered!\n");
}

int main(){
    pid_t pid=read_pid();
    printf(":::SENDER:::\n\n");
    printf("Sender PID: %d\n", getpid());
    printf("Connected with catcher PID: %d\n\n", pid);
    
    signal(SIGUSR1,handler);

    union sigval value;

    int mode=0;
    char input[MAX_BUFFER_SIZE];
    char option[MAX_BUFFER_SIZE]; 

    printf("ENTER SIGNAL MODE:\n");

    while(mode!=5){
        printf("--> ");
        fgets(input, sizeof(input), stdin);
        sscanf(input, "%s", option);
        mode=atoi(option);
        value.sival_int=mode;
        sigqueue(pid,SIGUSR1,value);
        pause();
    }
    return 0;    
}