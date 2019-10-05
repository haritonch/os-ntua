#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

typedef long long int ll;

char readbuffer[22];
char writebuffer[22];

ll str_to_int(char *a){

   int len=strlen(a);
   ll dec=0;

   for(int i=0; i<len; i++){
      dec = dec * 10 + ( a[i] - '0' );
   }

   return dec;
}

int main(int argc, char **argv){

   int status;
   int n = str_to_int(argv[1]), k = str_to_int(argv[2]);
   if (n<1 || n>10 || k<0){
      printf("Invalid arguments\n");
      return 1;
   }

   int a=k/n, b=k%n;

   pid_t c[n];
   int fd[n+1][2];

   for(int i=0; i<n+1; i++){
      pipe(fd[i]);
   }

   for(int i=0; i<n; i++){

      c[i]=fork();

      if(c[i] < 0){
         perror("Fork");
         exit(1);
      }

      else if(c[i]==0){
         //child code
         //first we close the pipes that are not going to be used
         //by the current process
         if(k==0){
            close(fd[0][1]);
            close(fd[n][0]);
            for(int j=1; j<n; j++){
               close(fd[j][0]);
               close(fd[j][1]);
            }
            if(i==0){
               read(fd[i][0],readbuffer,sizeof(readbuffer));
               ll res = str_to_int(readbuffer);
               printf("res = %lld from process %d\n", res, i);
               close(fd[0][0]);
               sleep(1);
               sprintf(writebuffer, "%lld", res);
               write(fd[n][1],writebuffer,sizeof(writebuffer));
               close(fd[n][1]);
            }
            exit(0);
         }
         else{
            //closing all unused pipes
            for (int j=0; j<n; j++){
               if(j!=i){
                  close(fd[j][0]);
               }
               if(j!=(i+1)%n){
                  close(fd[j][1]);
               }
            }

            close(fd[n][0]);
            if((i+1)%n!=b && !(b==0 && i==0))
               close(fd[n][1]);

            if (i<b){
               a++;
            }

            for(int j=1; j<=a; j++){
               //if (j*(i+1)>k) continue;
               read(fd[i][0], readbuffer, sizeof(readbuffer));
               ll res = str_to_int(readbuffer) * (i + 1 + (j-1)*n);
               printf("res = %lld from process %d\n", res, i);
               sprintf(writebuffer, "%lld", res);
               sleep(1);
               if(j==a && (i+1)%n==b){
                  write(fd[n][1],writebuffer,sizeof(writebuffer));
                  close(fd[n][1]);
               }
               else{
                  write(fd[(i+1)%n][1], writebuffer, sizeof(writebuffer));
               }
            }
            if((i+1)%n==b)
               close(fd[(i+1)%n][1]);
            close(fd[i][0]);
            exit(0);
         }
      }
   }

   sleep(1);
   //close the pipes that are not going
   //to be used by father
   for(int i=0; i<n+1; i++){
      if(i!=0)
      close(fd[i][1]);
      if(i<n)
      close(fd[i][0]);
   }
   //give 1 to the first process
   sprintf(writebuffer, "%d", 1);
   write(fd[0][1], writebuffer, sizeof(writebuffer));
   //wait for the last process to give the result
   read(fd[n][0],readbuffer,sizeof(readbuffer));
   printf("Final result : %lld\n",str_to_int(readbuffer));
   for(int i=0; i<n; i++){
      wait(&status);
   }
   //close the remaining open pipes
   close(fd[0][1]);
   close(fd[n][0]);

   return 0;
}
