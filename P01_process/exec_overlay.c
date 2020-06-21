// Using exec function
// This program is called by exec_start program

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	printf("In exec_overlay\n");
	printf("After the exec my process id is %d\n", getpid());
	printf("My parent process's id is %d\n", getppid());

	printf("\nexec ends\n");
	return 0;
}
