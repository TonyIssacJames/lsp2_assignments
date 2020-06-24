#include "sig.h"

int signal_register(int signum, void (*fun)(int), struct sigaction *oldact, 
                        sigset_t *sa_mask) 
{
    // TODO 1: Register the signal handler and save the current behaviour in oldact
	struct sigaction sa_new;
	
	memset(&sa_new, 0, sizeof(sa_new));
	
	if(fun != NULL)
	{
		sa_new.sa_sigaction = fun;
		sa_new.sa_mask = *sa_mask;
		sa_new.sa_flags |= SA_SIGINFO;
		
		sigaction(signum, &sa_new, oldact);
		return 0;
	}
	
	
    return -1;  //failure
}

int signal_deregister(int signum) 
{
    // TODO 2: Reset the behaviour to SIG_DFL
	struct sigaction sa_new;
	
	memset(&sa_new, 0, sizeof(sa_new));
	
	sa_new.sa_sigaction = SIG_DFL;
		
	sigaction(signum, &sa_new, NULL);
    return 0;
}

int signal_restore(int signum, struct sigaction *sa_old)
{
    // TODO 3: Restore the behaviour as per sa
	
	sigaction(signum, sa_old, NULL);
	
    return 0;
}

int signal_ignore(int signum) 
{
    // TODO 4: Ignore the signal
	struct sigaction sa_new;
	
	memset(&sa_new, 0, sizeof(sa_new));
	
	sa_new.sa_sigaction = SIG_IGN;
		
	sigaction(signum, &sa_new, NULL);

	
    return 0;
}

int signal_send(pid_t pid, int signum)
{
    // TODO 5: Send the signal to the process
	kill(pid,  signum);
    return 0;
}
