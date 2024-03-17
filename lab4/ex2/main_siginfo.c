#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


void sig_handler_info(int signo, siginfo_t *info, void *context) {
    printf("\nReceived signal ID: %d\nPID of sending process: %d\nReal user ID of sending process: %d\nSignal code: %d\nExit value: %d\n\n", signo, info->si_pid, info->si_uid, info->si_code, info->si_status);
    }


int main() {
    printf(":::SA_SIGINFO:::\n");
    printf("Parent PID: %d\n",getpid());
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = &sig_handler_info;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGALRM, &sa, NULL);
    fflush(NULL);
    pid_t pid=fork();
    if (pid==0){
        printf("Child PID: %d\n", getpid());
        kill(getppid(), SIGUSR1);
    }
    else{
        wait(NULL);
        raise(SIGUSR1);
        raise(SIGALRM);
    }
    return 0;
}
