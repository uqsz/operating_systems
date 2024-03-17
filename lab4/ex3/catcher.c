#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

int cnt=0;
int mode=0;

void success(int n){
    printf("Signal successfully executed with operating mode: %d.\n",n);
    printf("#################################################\n");
}

void show_numbers(){
    for (int i=1;i<101;i++){
        printf("%d ",i);
        if (i%10==0) printf("\n");
    }
    printf("\n");
}

void show_time(){
    time_t t;
    time(&t);
    printf("Current time: %s\n",ctime(&t));
}

void show_time_2(){
    time_t t;
    while (mode==4){
        time(&t);
        printf("Current time: %s\n",ctime(&t));
        sleep(1);
    }
}

void save_pid(){
    pid_t pid=getpid();
    printf(":::CATCHER:::\n\n");
    printf("Catcher PID: %d\n\n",pid);
    FILE *file = fopen("temp.txt", "w");
    fprintf(file, "%d", pid);
    fclose(file);
}

void handler(int signo, siginfo_t *info, void *context){
    mode=info->si_value.sival_int;
    printf("Received SIGUSR1 from PID: %d with operating mode: %d.\n\n",info->si_pid, mode);
    kill(info->si_pid,SIGUSR1);
    cnt++;
    fflush(stdout);
}


int main(){
    save_pid();
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = &handler;
    sigaction(SIGUSR1, &sa, NULL);

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask,SIGUSR1);

    while(1) {
        pause();
        if (mode==4){
            show_time_2();
            success(4);
            printf("Executing mode that interrupted mode 4.\n\n");
        }
        switch(mode){
        case 1:
            show_numbers();
            success(1);
            break;
        case 2:
            show_time();
            success(2);
            break;
        case 3:
            printf("Number of received requests since the beginning: %d\n\n",cnt);
            success(3);
            break;
        case 5:
            exit(0);
        default:
            printf("Invalid mode!");

    }
        sleep(1);
    }


}