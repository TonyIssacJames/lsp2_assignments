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
        //sleep(60); //so we can make main program wait
        // TODO 1: Wait for the child process termination
        cpid = wait(&status);
        // TODO 2: Print the exit code, if process exited normally
        if(WIFEXITED(status))
        {
            printf("Process: child exited normally\n");
            printf("Process: Exit Status %d\n",WEXITSTATUS(status));
        }
        // TODO 3: Print the signal number, if process terminated by signals
        if(WIFSIGNALED(status))
        {
            printf("Process: child exited because of a signal\n");
            printf("Process: Exit Status %d\n",WTERMSIG(status));
        }

        printf("Process: Child process id = %d\n",cpid);
        printf("\nProcess: Done with main program.\n");
    }

    printf("\nprocess id = %d\n",getpid());
    return 0;
}
