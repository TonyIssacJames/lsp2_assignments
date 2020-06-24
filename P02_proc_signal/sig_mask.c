#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "sig.h"

sig_atomic_t sigintcnt = 0;

void handler(int signum, siginfo_t  *info, void *context)
{
    ++sigintcnt;
    if (signum == SIGUSR1)
    {
		printf("SIGUSR1 sent from  %d ...\n", info->si_pid);
        printf("SIGUSR1: Sleeping ...sleeep for 10 sec\n");
        sleep(10);
        printf("SIGUSR1: Done\n");
    }
	
	 if (signum == SIGINT)
	 {
		  printf("SIGINT sent from  %d ...\n", info->si_pid);
		  printf("SIGINT: Handled ... sleeep for 10 sec\n");
		  sleep(10);
		  //printf("SIGINT: Done\n");
	 }
	 
	 printf("In handler: Signal Count: %d\n", (int)sigintcnt);
	 //printf("In handler: -------------------------------------------------------\n");
}

int main()
{
    sigset_t sa_mask;
	
	printf("The process id is %d\n", (int)getpid());
 	printf("The parent process id is %d\n", (int)getppid());
	
    // TODO 1: Register handler for SIGINT
    // TODO 2: Add SIGINT to sa_mask;
	if(sigemptyset(&sa_mask))
	{
		printf("Failed: error emptying mask\n");
	}

 	if(signal_register(SIGINT, handler, NULL, &sa_mask) )
	{
		printf("Failed: signal handler registration\n");
		return  -1;
	}
    // TODO 3: Register same handler for SIGUSR1 as well and pass &sa_mask as arg
	
	//if(sigaddset(&sa_mask, SIGINT))
	{
		printf("Failed: error addin mask\n");
	}
	
 	if(signal_register(SIGUSR1, handler, NULL, &sa_mask) )
	{
		printf("Failed: signal handler registration\n");
		return  -1;
	}
	
    for(;;)
    {
        printf("Signal Count: %d\n", (int)sigintcnt);	
        sleep(5); // Would return when signalled
    }
    return 0;
}
