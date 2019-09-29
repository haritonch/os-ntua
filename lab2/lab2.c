#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

pid_t c[6];
int child_number=-10;
int n=1;

void handler_cont(int sigint){
    return;
}

/*void handler_stop(int sigint){
    pause();
}*/

void justDoIt(int i){
    child_number=i;
    pause();
    while (1) {
        printf("Child%d %d is executed (%d)\n", child_number, (int) getpid(), n++);
        sleep(1);
    }
}

int main(int argc, char *argv[]) {

    signal(SIGCONT, handler_cont);
    //signal(SIGINT, handler_stop);
    int status = 0;
    int a[argc],count[argc];

    for(int i=1; i<=argc; i++){
      count[i]=0;
    }

    if(argc!=6){
        printf("Error : invalid arguments given\n");
        return 1;
    }

    for(int i=1; i<argc; i++) {
      a[i]=*argv[i]-'0';
      count[a[i]]++;
      if(count[a[i]]>1){
        printf("Error : invalid arguments given\n");
        return 1;
      }
    }

    printf("Parent PID: %d\n", (int) getpid());

    c[1]=fork();
    if (c[1]<0){
      perror("Fork");
      exit(1);
    }
    else if (c[1] == 0) {
        justDoIt(1);
    }

    c[3]=fork();
    if (c[3]<0){
      perror("Fork");
      exit(1);
    }
    else if (c[3] == 0) {
        justDoIt(3);
    }

    c[4]=fork();
    if (c[4]<0){
      perror("Fork");
      exit(1);
    }
    else if (c[4] == 0) {
        justDoIt(4);
    }

    c[2]=fork();
    if (c[2]<0){
      perror("Fork");
      exit(1);
    }
    else if (c[2] == 0) {
        justDoIt(2);
    }

    c[5]=fork();
    if (c[5]<0){
      perror("Fork");
      exit(1);
    }
    else if (c[5] == 0) {
        justDoIt(5);
    }

    usleep(1000); //1000us=0.001s , using usleep instead of sleep to handle milliseconds

    for (int r=0; r<4; r++) {
        for (int j = 1; j<=argc-1; j++) {
            kill(c[a[j]], SIGCONT);
            sleep(3);
            if (r<3){
                //kill(c[a[j]], SIGINT);
                kill(c[a[j]], SIGSTOP);
            }
            else{
                kill(c[a[j]],SIGKILL);
            }
        }
    }

    sleep(3);
    wait(&status);
    printf("exiting...\n");
    sleep(3);
    wait(&status);
    printf("exiting...\n");
    sleep(3);
    wait(&status);
    printf("exiting...\n");
    sleep(3);
    wait(&status);
    printf("exiting...\n");
    sleep(3);
    wait(&status);
    printf("exiting...\n");
    sleep(3);

    return 0;
}
