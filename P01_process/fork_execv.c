#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	int process_id;
	char *argv_list[] = {"-l", NULL};
	char *const envp[] = {"/bin/", NULL};

	process_id = fork();
    // TODO 1: Declare the argument list for "ls -l /"
    // TODO 2: Create the process with fork
    // TODO 3: Invoke execv in the child process with above argument list

    if(process_id == 0)
    {
    	printf("1 - We are in child\n");
    	printf("1 - The process id is %d\n", (int)getpid()); //printing current pid
    	printf("1 - The ppid is %d\n", (int)getppid());  //printing parent pid

    	execvpe("ls", argv_list, envp);

    	printf("1 - This won't print\n");
    }
    else
    {
    	sleep(5);
    	printf("2 - we are in current process\n");
    	printf("2 - The process id is %d\n", (int)getpid()); //printing current pid
    	printf("2- The ppid is %d\n", (int)getppid()); //printing parent pid

    }

    printf("\nexec ends for process(%d)\n----------------------\n",(int)getpid());
    return 0;
}
