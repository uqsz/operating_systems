#include <stdio.h>
#include <string.h>
#include <time.h>
#include <dirent.h>

// obliczanie rozmiaru pliku
long size_of_file(FILE *file)
{
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

// odwracanie napisu
char *reverse(char *str)
{
    size_t n = strlen(str);
    char tmp;
    for (int i = 0; i < n / 2; i++)
    {
        tmp = str[i];
        str[i] = str[n - 1 - i];
        str[n - 1 - i] = tmp;
    }
    return str;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Program requires 2 command line arguments: <char to find> <char to replace> <input file> <output file>\n");
        return 1;
    }

    clock_t t = clock();

    char *input_file = argv[1];
    char *output_file = argv[2];

    char buffer[BUFFER_SIZE + 1]; // +1 na znak zakończenia stringa

    FILE *input_fp = fopen(input_file, "r");
    FILE *output_fp = fopen(output_file, "w");

    long size = size_of_file(input_fp);
    size_t cnt = size / ((size_t)BUFFER_SIZE); // calkowita ilosc blokow
    size_t rest = size % ((size_t)BUFFER_SIZE); // modulo blokow

    size_t end;

    for (int i = 0; i <= cnt; i++)
    {
        fseek(input_fp, -BUFFER_SIZE * i, SEEK_END);
        end = fread(buffer, sizeof(char), BUFFER_SIZE, input_fp);
        buffer[end] = 0;
        fwrite(reverse(buffer), sizeof(char), end, output_fp);
    }

    fseek(input_fp, 0, SEEK_SET);
    end = fread(buffer, sizeof(char), rest, input_fp);
    buffer[end] = 0;
    fwrite(reverse(buffer), sizeof(char), end, output_fp);

    fclose(input_fp);
    fclose(output_fp);

    t = clock() - t;
    double time_taken = ((double)t) / CLOCKS_PER_SEC;
    printf("executed in %.4fs with %d buffer size\n", time_taken, BUFFER_SIZE);
}