#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "sig.h"

sig_atomic_t sigint_count = 0;
struct sigaction sa_org;

void handler(int signum)
{
    sigint_count++;
    printf("Handlere Called  %d\n", sigint_count);
	
	//Now lets degrister the signal
	//signal_deregister(signum) ;
	signal_restore(signum, &sa_org);
	 printf("Signal handler is set to default\n");
}

int main()
{
    int i, sleep_duration;

	sigset_t  sa_mask;
	
	sigemptyset(&sa_mask);
	
	
    // TODO 6: Register the handler SIGINT
	if(signal_register(SIGINT, handler, &sa_org, &sa_mask) )
	{
		printf("Failed: signal handler registration\n");
		return  -1;
	}

	sleep_duration =  5;
	
    printf("Sleep duration is %d seconds\n", sleep_duration);

    for(i = 0; i < 20; i++)
    {
        printf("Hmmmmm  %d\n", i);
        sleep(sleep_duration);
    }

    printf("\nSIGINT was raised %d times\n", sigint_count);
    return 0;
}
