// C program to implement one side of FIFO
// This side reads first, then reads
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

char buffer[200]="";

void get_time(){
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(buffer, sizeof(buffer), "%d.%m.%Y", tm);
}

int main(){
    get_time();
    printf("%s",buffer);
    
}