#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void writer(const char *message, int count, int fd)
{
    int len = strlen(message);
    for(; count > 0; --count)
    {
        if (write(fd, message, len) < 0)
        {
            printf("Unable to write\n");
            return;
        }
        sleep(2);
    }
}

void reader(int fd)
{
    char buffer[1024];
    while (read(fd, buffer, sizeof(buffer)) > 0)
        printf("%s", buffer);
}

int main()
{
    int fds[2];
    pid_t pid, cpid;
	int status;
	//const char *message = {"This is parent process writing to Child:\n \tHow are you?\n"};
	const char *message = {"Parent to Child:\n How are you?\n"};
	
	//printf("%s\n", message);
	printf("message len = %d\n", strlen(message));
    // TODO 1: Create the pipe
	if(pipe(fds))
	{
		printf("Process: Error Creating a pipe \n");
		return -1;
	}

    // TODO 2: Create the new process
	pid=fork();
	
	if(pid < 0) // process creation failied
	{
		printf("Process: Error Creating a process  \n");
		return -1;
	}
	
	
	 if (pid == 0)
    {
        printf("Child: Child process id = %d\n",getpid());
        printf("Child: Child process ppid = %d\n",getppid());
		
		if(close (fds[1]) == -1) //close  unsued write end
		{
			printf("Child: Error closing fds \n");
		    return  -1;
		}
		
		// TODO 3: Make a child read from the pipe
		 printf("Child: Reading from pipe: \n\n");
		reader(fds[0]);
		
		if(close (fds[0]) == -1) //close  read end after read
		{
			printf("Child: Error closing fds \n");
		    return  -1;
		}
    }
    else 
    {//Parent Process
		printf("Process: Child process id = %d\n",getpid());
        printf("Process: Child process ppid = %d\n",getppid());
		
		if(close (fds[0]) == -1) //close  unsued read end
		{
			 printf("Process: Error closing fds \n");
		     return  -1;
		}
		 // TODO 4: Make a parent to write to pipe
		 printf("Process: Writing to pipe: \n\n");
		 writer(message, 3, fds[1]); // write 3 times to the pipe
		 
		 if(close (fds[1]) == -1) //close  write end after write
		{
			printf("Process: Error closing fds \n");
		     return  -1;
		}
		 
		 cpid = wait(&status);
        //  Print the exit code, if process exited normally
        if(WIFEXITED(status))
        {
            printf("Process: child exited normally\n");
            printf("Process: Exit Status %d\n",WEXITSTATUS(status));
        }
        // Print the signal number, if process terminated by signals
        if(WIFSIGNALED(status))
        {
            printf("Process: child exited because of a signal\n");
            printf("Process: Exit Status %d\n",WTERMSIG(status));
        }
    }

    return 0;
}
