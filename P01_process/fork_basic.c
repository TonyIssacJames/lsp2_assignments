#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	int process_id;
    // TODO 1: Print the process id for this process
    printf("The process id is %d\n", (int)getpid());
    // TODO 2: Create the new process with fork()
    // TODO 3: Print the pid and ppid for both the processes and child pid for parent

    process_id = fork();

    printf("Child process id = %d\n", process_id); //printing child process id


    if(process_id == 0)
    {
    	printf("1 - We are in child\n");
    	printf("1 - The process id is %d\n", (int)getpid()); //printing current pid
    	printf("1 - The ppid is %d\n", (int)getppid());  //printing parent pid
    }
    else
    {
    	printf("2 - we are in current process\n");
    	printf("2 - The process id is %d\n", (int)getpid()); //printing current pid
    	printf("2- The ppid is %d\n", (int)getppid()); //printing parent pid

    }

    printf("\nexec ends for process(%d)\n----------------------\n",(int)getpid());

    return 0;
}
