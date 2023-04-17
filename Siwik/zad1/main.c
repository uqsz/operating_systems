#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sighandler(int signum, siginfo_t *info, void *ptr){
    printf("Received signal SIGUSR1 with value %d\n", info->si_value.sival_int);
}


int main(int argc, char* argv[]) {

    if(argc != 3){
        printf("Not a suitable number of program parameters\n");
        return 1;
    }

    struct sigaction action;
    action.sa_sigaction = &sighandler;

    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_SIGINFO;

    //..........


    int child = fork();
    if(child == 0) {
        sigaction(SIGUSR1, &action, NULL);
        sigset_t mask;
        sigfillset(&mask);
        sigdelset(&mask, SIGUSR1);

        //zablokuj wszystkie sygnaly za wyjatkiem SIGUSR1
        //zdefiniuj obsluge SIGUSR1 w taki sposob zeby proces potomny wydrukowal
        //na konsole przekazana przez rodzica wraz z sygnalem SIGUSR1 wartosc
        pause();
    }
    else {
        union sigval value;

        value.sival_int = atoi(argv[1]);
        sigqueue(child, atoi(argv[2]), value);
        //wyslij do procesu potomnego sygnal przekazany jako argv[2]
        //wraz z wartoscia przekazana jako argv[1]
    }

    return 0;
}
