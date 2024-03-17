#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#define FIFO_PATH "tmp/myfifo"
#define BUFF_SIZE 2048

char write_buff[BUFF_SIZE] = "";

double f(double x){
    return 4.0/(x*x+1);
}

double calc_integral(double a, double b, double dx){
    double sum=0;
    double x;
    for (x=a; x<b; x+=dx){
        sum+=f(x)*dx;
    }
    return sum;
}

int main(int argc, char* argv[]) {
    double dx = strtod(argv[1], NULL);
    double a = strtod(argv[2], NULL);
    double b = strtod(argv[3], NULL);

    double local_sum = calc_integral(a, b, dx);

    size_t size = snprintf(write_buff, BUFF_SIZE, "%.9f\n", local_sum);

    int fifo = open(FIFO_PATH, O_WRONLY);
    write(fifo, write_buff, size);
    close(fifo);
    return 0;
}