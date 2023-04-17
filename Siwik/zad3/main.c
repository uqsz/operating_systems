#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main ( int argc, char *argv[] )
{
    int i, pid;

    if(argc != 2){
        printf("Not a suitable number of program arguments");
        exit(2);
    }else {
       for (i = 0; i < atoi(argv[1]); i++) {
            pid=fork();
            if (pid==0){
                printf("Numer dziecka: %d, PID dziecka: %d, PID rodzica: %d\n", i, getpid(), getppid());
                fflush(NULL);
                exit(0);
            }
        //*********************************************************
        //Uzupelnij petle w taki sposob aby stworzyc dokladnie argv[1] procesow potomnych, bedacych dziecmi
        //   tego samego procesu macierzystego.
           // Kazdy proces potomny powinien:
               // - "powiedziec ktorym jest dzieckiem",
                //-  jaki ma pid,
                //- kto jest jego rodzicem
           //******************************************************
        }
    }
    while(wait(NULL)>0);
    return 0;
}
