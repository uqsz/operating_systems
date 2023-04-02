#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <sys/wait.h>

typedef enum {
    IGNORE,
    HANDLER,
    MASK,
    PENDING,
    INVALID,
} OPTION;

OPTION convert(const char* input){
    if(strcmp(input,"ignore")==0) return IGNORE;
    if(strcmp(input,"handler")==0) return HANDLER;
    if(strcmp(input,"mask")==0) return MASK;
    if(strcmp(input,"pending")==0) return PENDING;
    return INVALID;
}

void raise_sig(){
    printf("Raised SIGUSR1 in %d.\n",getpid());
    raise(SIGUSR1);
}

void sig_handler(int sig){
    printf("Received SIGUSR1 in %d.\n",getpid());
}

void set_mask(){
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &sa.sa_mask, NULL);
}

void check(){
    sigset_t sigset;
    sigpending(&sigset);
    if (sigismember(&sigset, SIGUSR1)) printf("Signal SIGUSR1 is pending in %d\n", getpid());
    else printf("Signal SIGUSR1 is not pending in %d\n", getpid());
}


int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Invalid amount of arguments! Usage: %s <ignore|handler|mask|pending>\n", argv[0]);
        return 1;
    }
    OPTION to_do=convert(argv[1]);
    printf(">>>Executing: %s in %s\n",argv[1],argv[0]);
    #ifndef EXEC 
        #ifndef FORK
            fflush(stdout);
            if (to_do==PENDING) check();
            else raise_sig();
            return 0;
        #endif
    #endif
    switch (to_do){
        case INVALID:
            fprintf(stderr, "Invalid input! Usage: %s <ignore|handler|mask|pending>\n", argv[0]);
            return 1;
        case IGNORE:
            signal(SIGUSR1, SIG_IGN);
            raise_sig();
            break;
        case HANDLER:
            signal(SIGUSR1, sig_handler);
            raise_sig();
            break;
        case MASK:
            set_mask();
            raise_sig();
            break;
        case PENDING:
            set_mask();
            raise_sig();
            check();
            break;
    }
    fflush(stdout);
    #ifdef FORK
    fflush(NULL);
    pid_t pid=fork();
    if (pid==0){
        if (to_do==PENDING) check();
        else raise_sig();
    }
    wait(NULL);  
    #endif
    #ifdef EXEC
        execl("./main_simple","./main_simple", argv[1], NULL);
    #endif

}