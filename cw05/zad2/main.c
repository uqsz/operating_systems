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
    for (x=a; x<=b; x+=dx){
        sum+=f(x)*dx;
    }
    return sum;
}

int main(){
    struct timespec startTime, endTime;
    double executionTime;

    int n=100;
    int a=0;
    int b=1;
    double dif=b-a;
    double dx=0.000001;
    double k=dif/n;
    double total_sum=0;

    clock_gettime(CLOCK_REALTIME, &startTime);

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
    printf("%.9f\n",total_sum);
    clock_gettime(CLOCK_REALTIME, &endTime);

    executionTime=(endTime.tv_sec-startTime.tv_sec) + (endTime.tv_nsec-startTime.tv_nsec)/1e9;
    printf("%.9f\n", executionTime);

    clock_gettime(CLOCK_REALTIME, &startTime);
    total_sum=calc_integral(0,1,dx);
    clock_gettime(CLOCK_REALTIME, &endTime);

    executionTime=(endTime.tv_sec-startTime.tv_sec) + (endTime.tv_nsec-startTime.tv_nsec)/1e9;
    printf("%.9f\n",total_sum);
    printf("%.9f\n", executionTime);
}