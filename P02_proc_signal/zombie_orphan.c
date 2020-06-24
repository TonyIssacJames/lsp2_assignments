#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int status, i;
    pid_t pid;
    pid_t cpid;


    printf("\nProcess: process id = %d\n",getpid());
    printf("Process: process ppid = %d\n",getppid());

    if ((pid=fork()) == 0)
    {
        printf("Child: Child process id = %d\n",getpid());
        printf("Child: Child process ppid = %d\n",getppid());
        for (i = 0; i < 30; i++)
        {
            sleep(1);
        }
    }
    else
    {
        sleep(60); //so we can make main program wait
		//cpid = wait(&status);
        printf("\nProcess: Done with main program.\n");
    }

    printf("\nprocess id = %d\n",getpid());
    return 0;
}
