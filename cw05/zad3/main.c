#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>


#define FIFO_PATH "tmp/myfifo"
#define BUFF_SIZE 2048

char write_buff[BUFF_SIZE] = "";
char read_buff[BUFF_SIZE] = "";

char arg2[BUFF_SIZE];
char arg3[BUFF_SIZE];

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

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Użycie: %s <szerokość_prostokąta> <liczba_procesów_potomnych>\n", argv[0]);
        return 1;
    }
    struct timespec start_real, end_real;

    clock_gettime(CLOCK_REALTIME, &start_real);

    double dx = atof(argv[1]);
    int n = atoi(argv[2]);
    double a=0,b=1;
    double k=(b-a)/n;

    mkfifo(FIFO_PATH, 0666);

    for (int i=0; i<n; i++){
        int pid=fork();
        if (pid==0){
            snprintf(arg2, BUFF_SIZE, "%.9f", a+i*k);
            snprintf(arg3, BUFF_SIZE, "%.9f", a+(i+1)*k);
            execl("./calc", "calc", argv[1], arg2, arg3, NULL);
        }
    }

    double total_sum = 0.0;
    
    int fifo = open(FIFO_PATH, O_RDONLY);
    int cnt = 0;

    char delim[] = "\n";
    char* token;

    while (cnt < n) {
        size_t size = read(fifo, read_buff, BUFF_SIZE);
        read_buff[size] = 0;
        token = strtok(read_buff, delim);
        for (;token; token = strtok(NULL, delim)) {
            total_sum += strtod(token, NULL);
            cnt++;
        }
    }
    close(fifo);
    clock_gettime(CLOCK_REALTIME, &end_real);
    double elapsed_real = (end_real.tv_sec - start_real.tv_sec) * 1e3 + (end_real.tv_nsec - start_real.tv_nsec)/1e6;
    printf("::TEST::\ndx: %s  n: %d\n",argv[1],n);
    printf("MUL_PROGRAM: %.9f %.2f ms\n",total_sum, elapsed_real);

    clock_gettime(CLOCK_REALTIME, &start_real);
    total_sum=calc_integral(0,1,dx);
    clock_gettime(CLOCK_REALTIME, &end_real);

    elapsed_real = (end_real.tv_sec - start_real.tv_sec) * 1e3 + (end_real.tv_nsec - start_real.tv_nsec)/1e6;

    printf("ONE_PROGRAM: %.9f %.2f ms\n\n",total_sum, elapsed_real);
    
    remove(FIFO_PATH);

}