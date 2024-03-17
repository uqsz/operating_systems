#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int sig) {
    printf("\nSignal %d received. If SA_NODEFER is on, you can send new SIGINT, despite the sleep command below!\n", sig);
    sleep(3);
}

int main() {
    printf(":::SA_NODEFER:::\n\n");
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sa.sa_flags = SA_NODEFER;  // Ustaw flagę SA_NODEFER

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction error");
        exit(EXIT_FAILURE);
    }

    printf("Press Ctrl+C to send SIGINT.\n");
    while(1) {
        printf("Waiting for signal...\n");
        sleep(1);
    }

    return 0;
}