#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
   if (argc!=2) {
      fprintf(stderr, "Invalid input\n");
      exit(EXIT_FAILURE);
   }
   
   printf("%s ", argv[0]);
   fflush(stdout);
   execl("/bin/ls", "ls", argv[1], NULL);

   exit(EXIT_SUCCESS);
}