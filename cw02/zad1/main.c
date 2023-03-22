#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#define BUFFER_SIZE 1048576

// funkcje z biblioteki C
void replace_C(char find, char replace, char *input_file, char *output_file)
{
    char buffer[BUFFER_SIZE];
    FILE *input_fp = fopen(input_file, "r");
    FILE *output_fp = fopen(output_file, "w");
    size_t size = fread(buffer, sizeof(char), sizeof(buffer), input_fp);
    for (int i = 0; i < size; i++)
    {
        if (buffer[i] == find)
        {
            buffer[i] = replace;
        }
    }
    fwrite(buffer, sizeof(char), size, output_fp);
    fclose(input_fp);
    fclose(output_fp);
}

// funkcje systemowe
void replace_SYS(char find, char replace, char *input_file, char *output_file)
{
    char c;
    int in, out;
    in = open(input_file, O_RDONLY);
    out = open(output_file, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    while (read(in, &c, 1) == 1)
    {
        if (c != find)
            write(out, &c, 1);
        else
            write(out, &replace, 1);
    }
    close(in);
    close(out);
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        printf("Program requires 4 command line arguments: <char to find> <char to replace> <input file> <output file>\n");
        return 1;
    }

    clock_t t = clock();

    char find = argv[1][0];
    char replace = argv[2][0];
    char *input_file = argv[3];
    char *output_file = argv[4];

    char *a;

    if (OPTION == 1)
    {
        a = "C";
        replace_C(find, replace, input_file, output_file);
    }
    else
    {
        a = "SYS";
        replace_SYS(find, replace, input_file, output_file);
    }
    t = clock() - t;
    double time_taken = ((double)t) / CLOCKS_PER_SEC;

    printf("executed in %.4fs with %s library\n", time_taken, a);
}