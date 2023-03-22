#ifdef USE_DLL
#include "libwc_so.h"
#else
#include "libwc.h"
#endif

#include <time.h>

#define MAX_SIZE 1000
#define MAX_BUFFER_SIZE 2048

int main()
{
    clock_t t;
    char input[MAX_BUFFER_SIZE];   // bufor na komendy użytkownika
    char command[MAX_BUFFER_SIZE]; // bufor dla typu komendy
    char value[MAX_BUFFER_SIZE];   // bufor dla argumentu komendy
    bool is_initialised = false;   // flaga z informacją, czy istnieje już zainicjalizowana tablica
    MemoryBlocks *arr = NULL;
    while (1)
    {
        printf("REPL>>>");
        fgets(input, sizeof(input), stdin);     // odczytanie komendy od użytkownika
        sscanf(input, "%s %s", command, value); // parsowanie komendy
        t = clock();

        // komenda exit
        if (strcmp(command, "exit") == 0)
        {
            break;
        }

        // komenda init
        if (strcmp(command, "init") == 0)
        {
            if (!is_initialised && (atoi(value) >= -1 && atoi(value) < MAX_SIZE))
            {
                is_initialised = true;
                arr = malloc(sizeof(MemoryBlocks));
                MemoryBlocks_init(arr, atoi(value));
            }
            else if (is_initialised)
            {
                perror("ARRAY IS ALREADY INITIALIZED");
                continue;
            }
            else
            {
                perror("WRONG ARGUMENT");
            }
        }

        else if (is_initialised)
        {
            // komenda count
            if (strcmp(command, "count") == 0)
            {
                MemoryBlocks_add(arr, value);
            }

            // komenda show
            else if (strcmp(command, "show") == 0)
            {
                printf("%s\n", MemoryBlocks_get(arr, atoi(value)));
            }

            // komenda delete
            else if (strcmp(command, "delete") == 0)
            {
                MemoryBlocks_delete(arr, atoi(value));
            }

            // komenda destroy
            else if (strcmp(command, "destroy") == 0)
            {
                MemoryBlocks_destroy(arr);
                is_initialised = false;
            }
            else
            {
                perror("WRONG COMMAND");
                continue;
            }
        }
        else
        {
            perror("ARRAY IS NOT INITIALIZED!");
            continue;
        }
        t = clock() - t;
        double time_taken = ((double)t) / CLOCKS_PER_SEC; // calculate the elapsed time
        printf("\n%s executed in %.4fs\n", command, time_taken);
    }
}