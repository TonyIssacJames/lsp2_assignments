#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "sig.h"

void handler(int signum, siginfo_t  *info, void *context)
{
    printf("Received signal\n");
	if (signum == SIGINT)
	 {
		printf("SIGINT sent from  %d ...\n", info->si_pid);
	 }
	 printf("Handled signal\n");
}

int main()
{
	sigset_t sa_mask;
	int status;
	
	printf("\nProcess: process id = %d\n",getpid());
    printf("Process: process ppid = %d\n",getppid());
	
    // TODO 1: Register the handler for SIGINT
	if(sigemptyset(&sa_mask))
	{
		printf("Failed: error emptying mask\n");
	}

	if(signal_register(SIGINT, handler, NULL, &sa_mask) )
	{
		printf("Failed: signal handler registration\n");
		return  -1;
	}
	
	status = pause();
	
	printf("Process status = %d\n", status);
		
    return 0;
}
