#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

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
    double dx = atof(argv[1]); // Szerokość prostokąta - dokładność obliczeń
    int n = atoi(argv[2]); // Liczba procesów potomnych

    double a=0,b=1;
    double k=(b-a)/n;
    double total_sum=0;

    struct timespec start_real, end_real;

    clock_gettime(CLOCK_REALTIME, &start_real);

    int pipes[n][2];
    for (int i=0; i<n; i++){
        pipe(pipes[i]);
    }

    for (int i=0; i<n; i++){
        int pid=fork();
        if (pid==0){
            close(pipes[i][0]);
            double local_sum=calc_integral(a+i*k, a+(i+1)*k, dx);
            write(pipes[i][1], &local_sum, sizeof(double));
            close(pipes[i][1]);
            exit(0);
        }
    }

    for (int i=0; i<n; i++){
        close(pipes[i][1]);
        double local_sum;
        read(pipes[i][0],&local_sum,sizeof(double));
        close(pipes[i][0]);
        total_sum+=local_sum;
    }

    while(wait(NULL)>0);
    printf("::TEST::\ndx: %s  n: %d\n",argv[1],n);

    clock_gettime(CLOCK_REALTIME, &end_real);
    double elapsed_real = (end_real.tv_sec - start_real.tv_sec) * 1e3 + (end_real.tv_nsec - start_real.tv_nsec)/1e6;

    printf("MUL_PROCESS: %.9f %.2f ms\n",total_sum, elapsed_real);

    clock_gettime(CLOCK_REALTIME, &start_real);
    total_sum=calc_integral(0,1,dx);
    clock_gettime(CLOCK_REALTIME, &end_real);

    elapsed_real = (end_real.tv_sec - start_real.tv_sec) * 1e3 + (end_real.tv_nsec - start_real.tv_nsec)/1e6;

    printf("ONE_PROCESS: %.9f %.2f ms\n\n",total_sum, elapsed_real);
    fflush(stdout);
}