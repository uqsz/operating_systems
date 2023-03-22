#include "libwc.h"
#include <sys/stat.h>

char command[2048] = "";

// funkcja tworząca i zwracająca strukture
MemoryBlocks MemoryBlocks_create(size_t size)
{
    return (MemoryBlocks){
        .t = calloc(size, sizeof(char *)),
        .max_size = size,
        .curr_size = 0,
    };
}

// funkcja przyjmująca i tworząca strukture
void MemoryBlocks_init(MemoryBlocks *mem_blocks, size_t size)
{
    mem_blocks->t = calloc(size, sizeof(char *));
    mem_blocks->max_size = size;
    mem_blocks->curr_size = 0;
}

// funkcja zapisująca wyniki komendy wc do pamięci
void MemoryBlocks_add(MemoryBlocks *mem_blocks, char *filename)
{
    char command[100];
    sprintf(command, "wc %s > tmp\\temp.txt", filename);
    system(command);
    FILE *temp_file = fopen("tmp\\temp.txt", "r");
    struct stat st;
    stat("tmp\\temp.txt", &st);
    off_t size = st.st_size;
    void *block = calloc(size, sizeof(char));
    fread(block, sizeof(char), size, temp_file);
    fclose(temp_file);
    remove("tmp\\temp.txt");
    mem_blocks->t[mem_blocks->curr_size++] = block;
}

// funkcja zwracająca zawartość bloku o zadanym indeksie
char *MemoryBlocks_get(MemoryBlocks *mem_blocks, size_t block_idx)
{
    if (block_idx >= mem_blocks->curr_size)
    {
        perror("Index out of range.");
        return "";
    }
    return mem_blocks->t[block_idx]; // zwracanie wskaźnika na blok pamięci
}

// funkcja usuwająca blok o zadanym indeksie
void MemoryBlocks_delete(MemoryBlocks *mem_blocks, size_t block_idx)
{
    if (block_idx >= mem_blocks->curr_size)
    {
        perror("Index out of range.");
        return;
    }
    free(mem_blocks->t[block_idx]);
    mem_blocks->t[block_idx] = NULL;
}

// funkcja zwalniająca pamięć zaalokowaną dla bloków pamięci
void MemoryBlocks_destroy(MemoryBlocks *mem_blocks)
{
    for (size_t i = 0; i < mem_blocks->curr_size; i++)
    {
        free(mem_blocks->t[i]);
    }
    free(mem_blocks->t);
    free(mem_blocks);
}