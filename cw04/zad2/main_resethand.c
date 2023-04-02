#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void sigint_handler(int sig) {
    printf("\nReceived SIGINT\n");
}

int main() {
    printf(":::SA_RESETHAND:::\n\n");
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESETHAND;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    printf("Waiting for SIGINT...\n");
    pause();
    printf("SIGINT is reseted. The message \"Exit.\" will not be displayed because of SA_RESETHAND.\n");
    pause();
    printf("Exit.\n");
    return 0;
}