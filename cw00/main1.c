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

int x=0;

void handle_sigusr1(int sig)
{
    if (x==0){
    printf("RTime is up!\n");
    exit(0);
    }
}

int main() {
    pid_t pid = fork();
    if (pid == 0) {
        // Proces potomny
        sleep(3);
        //kill(getppid(),SIGUSR1);
    } else if (pid > 0) {
        signal(SIGUSR1,handle_sigusr1);
        // Proces rodzicielski
        int x;
        printf("Ile to 3*5: \n");
        scanf("%d",&x);
        alarm(3);
        if (x==15){
            printf("Right\n");
        }
        else{
            printf("Wrong!\n");
        }
        wait(NULL);
    } else {
        printf("Błąd podczas tworzenia procesu potomnego.\n");
        exit(1);
    }
    pause();

    return 0;
}
