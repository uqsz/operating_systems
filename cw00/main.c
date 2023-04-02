#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig)
{
    printf("Received signal SIGUSR2%d\n", sig);
}

void c(int sig){
    printf("Received signal SIGUSR1 %d\n", sig);
}

int main()
{
    sigset_t set, oldset;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    signal(SIGUSR1,c);
    signal(SIGUSR2,handler);
    if (sigprocmask(SIG_BLOCK, &set, &oldset) == -1) {
        perror("sigprocmask failed");
        return 1;
    }

    printf("Masked SIGINT. Waiting for signal...\n");
    pid_t pid=fork();
    if (pid==0){
        
            //sleep(2);
            //kill(getppid(), SIGCONT);
            sleep(2);
            kill(getppid(), SIGUSR2);
             kill(getppid(), SIGUSR1);
            sleep(2);
            kill(getppid(), SIGUSR1);
        
    }
    else{
        sleep(3);
        printf("Unmasked SIGINT. Waiting for signal...\n");

        if (sigprocmask(SIG_UNBLOCK, &set, NULL) == -1) {
            perror("sigprocmask failed");
            return 1;
        }

    }
        pause();
        pause();
    return 0;
}