#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
   if (argc!=2) {
      fprintf(stderr, "Invalid input\n");
      exit(EXIT_FAILURE);
   }

   int n=atoi(argv[1]);
   
   for (int i=0; i<n; ++i){
      pid_t pid = fork();
      if(pid==0) {
         printf("Proces macierzysty: %d, proces potomny: %d\n", getppid(), getpid());
         exit(EXIT_SUCCESS);
      }
   }

   while (wait(NULL)>0);
   printf("%s\n", argv[1]);
   exit(EXIT_SUCCESS);
}