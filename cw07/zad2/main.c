#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
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

int main() {
    const char* shm_name = "/salon_fryzjerski";

    // Tworzenie i otwieranie segmentu pamięci współdzielonej dla danych salonu fryzjerskiego
    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Błąd shm_open");
        exit(1);
    }

    // Ustawianie rozmiaru segmentu pamięci współdzielonej
    if (ftruncate(shm_fd, sizeof(struct salon_fryzjerski)) == -1) {
        perror("Błąd ftruncate");
        exit(1);
    }

    // Mapowanie segmentu pamięci współdzielonej do przestrzeni adresowej procesu
    struct salon_fryzjerski *salon = mmap(NULL, sizeof(struct salon_fryzjerski), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (salon == MAP_FAILED) {
        perror("Błąd mmap");
        exit(1);
    }

    // Inicjalizacja danych salonu fryzjerskiego
    salon->liczba_fryzjerow = M;
    salon->liczba_foteli = N;
    salon->liczba_miejsc_w_poczekalni = K;
    salon->liczba_fryzur = F;
    salon->liczba_klientow_w_poczekalni = 0;

    // Inicjalizacja semaforów
    sem_t *semafor_salon = sem_open("/semafor_salon", O_CREAT | O_RDWR, 0666, 1);
    sem_t *semafor_poczekalnia = sem_open("/semafor_poczekalnia", O_CREAT | O_RDWR, 0666, 1);
    if (semafor_salon == SEM_FAILED || semafor_poczekalnia == SEM_FAILED) {
        perror("Błąd sem_open");
        exit(1);
    }

    // Tworzenie procesów
    int i;
    for (i = 0; i < M; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Proces fryzjera
            while (1) {
                sem_wait(semafor_salon);

                // Sprawdzenie, czy są klienci w poczekalni
                if (salon->liczba_klientow_w_poczekalni > 0) {
                    salon->liczba_klientow_w_poczekalni--;
                    obsluga_klienta(i + 1);
                    sem_post(semafor_poczekalnia);
                } else {
                    sem_post(semafor_salon);
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
        sem_wait(semafor_salon);

        // Sprawdzenie dostępności fotela w poczekalni
        if (salon->liczba_klientow_w_poczekalni < K) {
            salon->liczba_klientow_w_poczekalni++;
            sem_post(semafor_salon);

            sem_wait(semafor_poczekalnia);
            // Klient czeka, aż zostanie obsłużony
            sem_post(semafor_salon);
        } else {
            // Brak miejsc w poczekalni, klient odchodzi
            sem_post(semafor_salon);
            sleep(1);   // Klient wraca później
        }
    }

    // Zamykanie i usuwanie zasobów
    sem_close(semafor_salon);
    sem_close(semafor_poczekalnia);
    sem_unlink("/semafor_salon");
    sem_unlink("/semafor_poczekalnia");

    munmap(salon, sizeof(struct salon_fryzjerski));
    shm_unlink(shm_name);

    return 0;
}
