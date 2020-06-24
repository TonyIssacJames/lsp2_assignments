#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int status, w, pid;
	pid_t cpid;
	
	printf("\nProcess: process id = %d\n",getpid());
    printf("Process: process ppid = %d\n",getppid());

    if ((pid = fork()) == 0)
    {
		printf("Child: Child process id = %d\n",getpid());
        printf("Child: Child process ppid = %d\n",getppid());
		
        pause();
    }
    else
    {
		printf("=============================\n");
        do {
				// TODO 1: Wait for the child with waitpid
				// Make sure to include WUNTRACED and WCONTINUE options
				cpid = waitpid(pid, &status, WUNTRACED|WCONTINUED);
				
				
				// TODO 2: Check if exited normally or abnormally and print status accordingly
				
				if(WIFEXITED(status))
				{
					printf("Process: child exited normally\n");
					printf("Process: Exit Status %d\n",WEXITSTATUS(status));
				}
				else
				{
					printf("Process: child exited abnormally\n");
					printf("Process: Exit Status %d\n",WEXITSTATUS(status));					
				}
				// TODO 3: Print the signal number, if process terminated by signals
				if(WIFSIGNALED(status))
				{
					printf("Process: child exited because of a signal\n");
					printf("Process: Exit Status %d\n",WTERMSIG(status));
				}
						
				// TODO 3: Check for the stop and continue signals		
				if(WIFCONTINUED(status))
				{
					printf("Process: The Child process Continues\n");
				}

				if(WIFSTOPPED(status))
				{
					printf("Process: The Child process Stopped\n");
				}
				
			printf("-----------------------------------------\n");				

            } while (!WIFEXITED(status) && !WIFSIGNALED(status));

        printf("\nProcess: Done with main program.\n");
    }
    return 0;
}
