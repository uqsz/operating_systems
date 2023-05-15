/* napisz program, w którym stworzysz 5 wątków (parametr programu) w którym kazdy w nieskonczonej
petli wypisuje na konsole napis typu "Watek n. Iteracja m". Dodatkowo po stworzeniu wszystkich watkow program glowny
takze (w nieskonczonej petli) wypisuje na konsole napis typu "Hello tu program głowny Iteracja m" */

/* zmodyfikuj poprzednie w taki sposob ze program glowny zabija w sposob asynchroniczny wszystkie watki po wypisaniu 
na konsole swojego napisu 10 razy*/

/*w watku masz miec dwa miejsca drukowania komunikatu na konsole nastepnie program glowny zabija swoje watki w sposob 
synchroniczny po wypiasniu n razy swojego komunikatu ustaw punkt synchornizacji na miejsce pomiedzy wypisaniem pierwszego
 i drugiego komunikatu na konsole*/

 /*napisz program w ktorym utworzysz watek ktory wypisze na konsole napis typu(Hello tu watek iteracja N
  (N parametr programu) i "sie zakonczy" program glowny powinien zaczekac na zakonczenie sie swojego watku debrac wartosc
   zwrocona przez watek i wypisac po tym potwierdzenie typu watek zakonczony konczymy wartosc zwrocona przez watek to:*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int num_threads = 5;
pthread_t threads[5];
int thread_nums[5];

void* thread_function(void* arg) {
    int thread_num = *(int*) arg;
    int i = 1;
    while (i < 11) {
        printf("Watek %d. Iteracja %d\n", thread_num, i);
        i++;
        sleep(2);
    }
}

void kill_threads() {
    for (int i = 0; i < num_threads; i++) {
        pthread_cancel(threads[i]);
    }
}

int main() {
    for (int i = 0; i < num_threads; i++) {
        thread_nums[i] = i + 1;
        pthread_create(&threads[i], NULL, thread_function, (void*) &thread_nums[i]);
    }

    int j = 1;
    while (1) {
        printf("Hello tu program glowny. Iteracja %d\n", j);
        if (j == 10) {
            kill_threads();
        }
        j++;
        sleep(1.1);
    }
    return 0;
}






