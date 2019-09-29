#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void print(void){
	for(int i=0; i<10; i++){
		printf("Process %d is executed, my father is %d\n",getpid(),getppid());
		sleep(1);
	}
}

int main(){

	int status;
	pid_t c1,c2;
	c1=fork(); //Child 1 process created
	if(c1<0){
		printf("Error : Fork Failed\n");
	}
	if(c1==0){
		//Child 1 code goes here
		pid_t c3,c4;

		c3=fork(); //Child 3 process created (father Child 1)

		if(c3<0){
			printf("Error - Fork Failed\n");
		}
		else if(c3==0){
			//Child 3 code goes here
			print();
			exit(0);
		}

		c4=fork(); //Child 4 process created (father Child 1)
		if(c4<0){
			printf("Error - Fork Failed\n");
		}
		if(c4==0){
			//Child 4 code goes here
			print();
			exit(0);
		}
		//Father(Child 1) code goes here
		print();
		wait(&status); //waiting for Child 3 and 4 processes to finish
		exit(0);
		
	}
	//we can wait for Child 3 and Child 4 processes to end to continue to Child 2,
	//but our programm is not parallel
	/*else{
		wait(&status);
	}*/
	c2=fork(); //Child 2 process created
	if(c2<0){
		printf("Error - Fork Failed\n");
	}
	else if(c2==0){
		//Child 2 code goes here
		pid_t c5;
		c5=fork(); //Child 5 process created (father is Child 2)
		if(c5<0){
			printf("Error - Fork Failed");
		}
		else if(c5==0){
			//Child 5 code goes here
			print();
			exit(0);
		}
		else{
			//Father (Child 2) code goes here
			print();
			wait(&status); //waiting for Child 5 processes to finish
			exit(0);
		}
	}
	//Father process, we don't have to print anything
	wait(&status);
	exit(0);

	return(0);
}
