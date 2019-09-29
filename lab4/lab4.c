#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

void semaphoreWait(int *s){
    char ctr;
    read(s[0],&ctr,sizeof(char));
    return;
}

void semaphoreSignal(int *s){
    char ctr='X';
    write(s[1],&ctr,sizeof(char));
    return;
}

void critical_section(int c, int *fd){
    semaphoreWait(fd);
    for (int j=0; j<5; j++){
        printf("Child%d %d executes a critical section\n", c+1, getpid());
    }
    semaphoreSignal(fd);
}

void non_critical_section(int child_number){
    for (int j=0; j<7; j++){
        printf("Child%d %d executes a non critical section\n", child_number+1, getpid());
    }
}

void parse_process(int process_number, char *s, int *fd){
    for (int i=0; i<strlen(s); i++){
        if (s[i] == '0') non_critical_section(process_number);
        else critical_section(process_number, fd);
    }
}


int main(){

    int status = 0;
    pid_t child[3];
    int fd[2];

    if ( pipe(fd) == -1){
        printf("Error\n");
        return 1;
    }

    for (int i=0; i<3; i++){
        child[i] = fork();
        if (child[i] < 0){
            printf("Fork failed\n");
            exit(1);
        }
        else if (child[i] == 0){
            //child code

            if (i==0){
                // Child 1
                /*critical_section(i, fd);
                non_critical_section(i);
                non_critical_section(i);
                critical_section(i, fd);
                non_critical_section(i);
                critical_section(i, fd);*/
                parse_process(i, "100101", fd);

                close(fd[0]);
                close(fd[1]);
                exit(0);
            }
            else if (i==1){
                // Child 2
                /*non_critical_section(i);
                critical_section(i, fd);
                non_critical_section(i);
                critical_section(i, fd);
                non_critical_section(i);
                critical_section(i, fd);*/
                parse_process(i, "010101", fd);

                close(fd[0]);
                close(fd[1]);
                exit(0);
            }

            else if (i==2){
                // Child 3
                /*critical_section(i, fd);
                non_critical_section(i);
                critical_section(i, fd);
                non_critical_section(i);
                critical_section(i, fd);
                non_critical_section(i);*/
                parse_process(i, "101010", fd);

                close(fd[0]);
                close(fd[1]);
                exit(0);
            }
        }
    }

    semaphoreSignal(fd);

    wait(&status);
    wait(&status);
    wait(&status);

    return 0;
}
