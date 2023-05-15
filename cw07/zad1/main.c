#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

#define M 3     // Liczba fryzjerów
#define N 2     // Liczba foteli
#define K 5     // Liczba miejsc w poczekalni
#define F 3     // Liczba dostępnych fryzur

// Struktura przechowująca dane dotyczące salonu fryzjerskiego
struct salon_fryzjerski {
    int liczba_fryzjerow;
    int liczba_foteli;
    int liczba_miejsc_w_poczekalni;
    int liczba_fryzur;
    int liczba_klientow_w_poczekalni;
};

// Funkcja do obsługi klienta przez fryzjera
void obsluga_klienta(int fryzjer_id) {
    printf("Fryzjer %d obsługuje klienta\n", fryzjer_id);
    sleep(rand() % 5);    // Symulacja czasu obsługi klienta
}

// Funkcja wykonująca operację P() na semaforze o podanym identyfikatorze
void P(int semid) {
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = -1;
    buf.sem_flg = 0;
    semop(semid, &buf, 1);
}

// Funkcja wykonująca operację V() na semaforze o podanym identyfikatorze
void V(int semid) {
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = 1;
    buf.sem_flg = 0;
    semop(semid, &buf, 1);
}

int main() {
    key_t klucz = ftok(".", 'A');   // Generowanie unikalnego klucza

    // Tworzenie segmentu pamięci współdzielonej dla danych salonu fryzjerskiego
    int id_pamieci = shmget(klucz, sizeof(struct salon_fryzjerski), IPC_CREAT | 0666);
    if (id_pamieci == -1) {
        perror("Błąd shmget");
        exit(1);
    }

    // Przyłączanie segmentu pamięci współdzielonej
    struct salon_fryzjerski *salon = shmat(id_pamieci, NULL, 0);
    if (salon == (void *) -1) {
        perror("Błąd shmat");
        exit(1);
    }

    // Inicjalizacja danych salonu fryzjerskiego
    salon->liczba_fryzjerow = M;
    salon->liczba_foteli = N;
    salon->liczba_miejsc_w_poczekalni = K;
    salon->liczba_fryzur = F;
    salon->liczba_klientow_w_poczekalni = 0;

    // Inicjalizacja semaforów
    int semafor_salon_id = semget(klucz, 1, IPC_CREAT | 0666);
    int semafor_poczekalnia_id = semget(klucz + 1, 1, IPC_CREAT | 0666);

    // Ustawienie wartości początkowej semaforów
    semctl(semafor_salon_id, 0, SETVAL, 1);           // Semafor do synchronizacji dostępu do danych salonu
    semctl(semafor_poczekalnia_id, 0, SETVAL, 1);     // Semafor do synchronizacji dostępu do poczekalni

    // Tworzenie procesów fryzjerów
    int i;
    for (i = 0; i < M; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Proces fryzjera
            while (1) {
                P(semafor_salon_id);

                // Sprawdzenie, czy są klienci w poczekalni
                if (salon->liczba_klientow_w_poczekalni > 0) {
                    salon->liczba_klientow_w_poczekalni--;
                    obsluga_klienta(i + 1);
                    V(semafor_poczekalnia_id);
                } else {
                    V(semafor_salon_id);
                    sleep(1);   // Fryzjer czeka na klientów
                }
            }
            exit(0);
        } else if (pid == -1) {
            perror("Błąd fork");
            exit(1);
        }
    }

    // Proces klienta
    while (1) {
        P(semafor_salon_id);

        // Sprawdzenie dostępności fotela w poczekalni
        if (salon->liczba_klientow_w_poczekalni < K) {
            salon->liczba_klientow_w_poczekalni++;
            V(semafor_salon_id);

            P(semafor_poczekalnia_id);
            // Klient czeka, aż zostanie obsłużony
            V(semafor_salon_id);
        } else {
            // Brak miejsc w poczekalni, klient odchodzi
            V(semafor_salon_id);
            sleep(1);   // Klient wraca później
        }
    }

    // Usunięcie semaforów
    semctl(semafor_salon_id, 0, IPC_RMID);
    semctl(semafor_poczekalnia_id, 0, IPC_RMID);

    return 0;
}