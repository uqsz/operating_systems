#include "grid.h"
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>

const int grid_width = 30;
const int grid_height = 30;

char *create_grid()
{
    return malloc(sizeof(char) * grid_width * grid_height);
}

void destroy_grid(char *grid)
{
    free(grid);
}

void draw_grid(char *grid)
{
    for (int i = 0; i < grid_height; ++i)
    {
        // Two characters for more uniform spaces (vertical vs horizontal)
        for (int j = 0; j < grid_width; ++j)
        {
            if (grid[i * grid_width + j])
            {
                mvprintw(i, j * 2, "■");
                mvprintw(i, j * 2 + 1, " ");
            }
            else
            {
                mvprintw(i, j * 2, " ");
                mvprintw(i, j * 2 + 1, " ");
            }
        }
    }

    refresh();
}

void init_grid(char *grid)
{
    for (int i = 0; i < grid_width * grid_height; ++i)
        grid[i] = rand() % 2 == 0;
}

bool is_alive(int row, int col, char *grid)
{

    int count = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
            {
                continue;
            }
            int r = row + i;
            int c = col + j;
            if (r < 0 || r >= grid_height || c < 0 || c >= grid_width)
            {
                continue;
            }
            if (grid[grid_width * r + c])
            {
                count++;
            }
        }
    }

    if (grid[row * grid_width + col])
    {
        if (count == 2 || count == 3)
            return true;
        else
            return false;
    }
    else
    {
        if (count == 3)
            return true;
        else
            return false;
    }
}

typedef struct {
    int start;
    int end;
    char* src;
    char* dst;
} CellData;

void handler(int signo) {}

void* update_cell(void* _thread_data) {
    CellData* thread_data = (CellData*) _thread_data;
    while (1) {
        for (int i = thread_data->start; i < thread_data->end; i++) {
            thread_data->dst[i] = is_alive(i / grid_width, i % grid_width, thread_data->src);
        }
        pause();

        char* tmp = thread_data->src;
        thread_data->src = thread_data->dst;
        thread_data->dst = tmp;
    }

    return NULL;
}

void update_grid(char* src, char* dst, int n) {
    static int num_threads = -1;
    static pthread_t* thread_data = NULL;

    signal(SIGUSR1, handler);

    //inicjalizacja wątków, jezeli thread_data nie zostało zainicjalizowane (jest NULL)
    if (!thread_data) {
        int num_cells = grid_width * grid_height;
        if (n < 0) { //gdy n=-1, ilosc watkow rowna sie ilosci komorek
            num_threads = num_cells;
        } else {
            num_threads = n;
        }

        thread_data = malloc(sizeof(pthread_t) * num_threads);

        // liczba komorek dla jednego watku
        int interval;
        if (num_cells % num_threads != 0) { // dodanie do kazdege watku jednej komorki -> rownomierny podzial
            interval = num_cells / num_threads + 1;
        } else {
            interval = num_cells / num_threads;
        }

        for (int i = 0; i < num_cells; i += interval) {
            CellData* data = malloc(sizeof(CellData));
            data->start = i;
            if (i + interval + interval > num_cells) {
                data->end = num_cells;
            } else {
                data->end = i + interval;
            }
            data->src = src;
            data->dst = dst;

            //stworzenie watku
            pthread_create(thread_data + (i / interval), NULL, update_cell, (void*)data);
        }
    }

    // wyslanie sygnalow do watkow, zeby wylaczyc pause()
    for (int i = 0; i < num_threads; i++) {
        pthread_kill(thread_data[i], SIGUSR1);
    }
}


