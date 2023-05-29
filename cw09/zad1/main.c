#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/syscall.h>

#define REINDEER_TOTAL 9
#define ELVES_TOTAL 10
#define ELVES_REQUIRED 3

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t condition;
    unsigned int elvesCount;
    unsigned int reindeerCount;
} Monitor;

static Monitor monitor = {
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .condition = PTHREAD_COND_INITIALIZER,
    .reindeerCount = 0,
    .elvesCount = 0
};

static pthread_barrier_t reindeerBarrier, elvesBarrier;

pid_t getThreadId() {
    return syscall(SYS_gettid); 
}

void *reindeerRoutine(void *arg) {
    while (true) {
        printf("Renifer %d jest na wakacjach\n", getThreadId());
        sleep(5 + (rand() % 6));

        pthread_mutex_lock(&monitor.mutex);
        monitor.reindeerCount++;
        if (monitor.reindeerCount == REINDEER_TOTAL) {
            printf("Renifer %d budzi Mikołaja\n", getThreadId());
            pthread_cond_broadcast(&monitor.condition);
        } else {
            printf("Renifer %d wraca z wakacji\n", getThreadId());
        }
        pthread_mutex_unlock(&monitor.mutex);

        pthread_barrier_wait(&reindeerBarrier);
    }
    return NULL;
}

void *elfRoutine(void *arg) {
    while (true) {
        sleep(2 + (rand() % 4));

        pthread_mutex_lock(&monitor.mutex);
        if (monitor.elvesCount < ELVES_REQUIRED) {
            monitor.elvesCount++;
            if (monitor.elvesCount == ELVES_REQUIRED) {
                printf("Elf %d budzi Mikołaja\n", getThreadId());
                pthread_cond_broadcast(&monitor.condition);
            } else {
                printf("Elf %d czeka na Mikołaja\n", getThreadId());
            }
        } else {
            printf("Elf %d radzi sobie sam\n", getThreadId());
        }
        pthread_mutex_unlock(&monitor.mutex);

        pthread_barrier_wait(&elvesBarrier);
    }
    return NULL;
}

int main(int argc, char** argv) {
    srand(time(NULL));

    pthread_barrier_init(&reindeerBarrier, NULL, REINDEER_TOTAL + 1);
    pthread_barrier_init(&elvesBarrier, NULL, ELVES_REQUIRED + 1);

    pthread_t reindeerIds[REINDEER_TOTAL];
    for (int i = 0; i < REINDEER_TOTAL; i++) {
        pthread_create(&reindeerIds[i], NULL, reindeerRoutine, NULL);
    }

    pthread_t elfIds[ELVES_TOTAL];
    for (int i = 0; i < ELVES_TOTAL; i++) {
        pthread_create(&elfIds[i], NULL, elfRoutine, NULL);
    }

    while (true) {
        pthread_mutex_lock(&monitor.mutex);
        while (monitor.reindeerCount < REINDEER_TOTAL && monitor.elvesCount < ELVES_REQUIRED) {
            pthread_cond_wait(&monitor.condition, &monitor.mutex);
        }
        if (monitor.reindeerCount == REINDEER_TOTAL) {
            printf("Mikolaj sie obudzil i dostarcza prezenty\n");
            sleep(2 + rand() % 3);
            monitor.reindeerCount = 0;
            pthread_barrier_wait(&reindeerBarrier);
        } else if (monitor.elvesCount == ELVES_REQUIRED) {
            printf("Mikolaj sie obudzil i pomaga elfom\n");
            sleep(1 + rand() % 2);
            monitor.elvesCount = 0;
            pthread_barrier_wait(&elvesBarrier);
        } else {
            printf("Error\n");
        }
        pthread_mutex_unlock(&monitor.mutex);
    }

    return EXIT_SUCCESS;
}
