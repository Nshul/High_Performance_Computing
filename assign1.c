#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
	pid_t pid,pid2;
	int var=2;

	printf("This is the parent process with PID=%d and val of var=%d\n",getpid(),var);
	switch(pid=fork()){
		case -1:// Couldn't Fork
				printf("Couldn't fork child process\n");break;
		case 0:// Successfull Fork
			var++;
			sleep(2); //Sleeping this child process for 2 seconds
			printf("**\n");
			printf("This is First child process having pid as %d\n", getpid());
			printf("Parent's PID:%d\n",getppid());
			printf("Value of var=%d\n",var);printf("**\n"); break;
		default: //back to parent process
			pid2 = wait(NULL);// Waiting for child process
			printf("Returned from process %d\n",pid2);
			printf("Back to Parent process having PID as %d ", getpid());
			printf("Child's PID:%d\n",pid);
			printf("Value of var=%d\n",var);printf("**\n"); 
			switch(pid=fork()){
				case -1://Couldn't Fork
					printf("Couldn't fork child process\n");break;
				case 0:// Successfull Fork
					var++;
					printf("**\n");
					printf("This is Second child process having PID as %d ", getpid());
					printf("Parent's PID:%d\n",getppid());
					exit(0);
					// This will not be displayed on shell
					printf("Value of var=%d\n",var);printf("**\n"); break;
				default: //back to parent process
					// pid2 = wait(NULL);// Waiting for child process
					// Here not waiting for child so parent process can run before child process
					printf("Back to Parent process having PID as %d ", getpid());
					printf("Child's PID:%d\n",pid);
					printf("Value of var=%d\n",var);printf("**\n"); 
			}
	}
	return 0;
}
