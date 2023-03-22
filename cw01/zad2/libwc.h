#ifndef LIBWC_H
#define LIBWC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <string.h>

typedef struct
{
    char **t;         // tablica wskaznikow na blok pamieci
    size_t max_size;  // maksymalna ilosc blokow, jakie mozna zapisac
    size_t curr_size; // aktualna ilosc zapisanych blokow
} MemoryBlocks;

// funkcja tworząca i zwracająca strukture
MemoryBlocks MemoryBlocks_create(size_t max_blocks);

// funkcja przyjmująca i tworząca strukture
void MemoryBlocks_init(MemoryBlocks *mem_blocks, size_t max_blocks);

// funkcja zapisująca wyniki komendy wc do pamięci
void MemoryBlocks_add(MemoryBlocks *mem_blocks, char *filename);

// funkcja zwracająca zawartość bloku o zadanym indeksie
char *MemoryBlocks_get(MemoryBlocks *mem_blocks, size_t block_idx);

// funkcja usuwająca blok o zadanym indeksie
void MemoryBlocks_delete(MemoryBlocks *mem_blocks, size_t block_idx);

// funkcja zwalniająca pamięć zaalokowaną dla bloków pamięci
void MemoryBlocks_destroy(MemoryBlocks *mem_blocks);

#endif