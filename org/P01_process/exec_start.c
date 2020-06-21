// Using exec function
// This program calls exec_overlay program using exec function

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main()
{
	//char *argv_list[] = {NULL};
	char *argv_list[] = {"-l", NULL};
	char *const envp[] = {"/bin/", NULL};

	printf("Before exec my id is %d\n", getpid());
	printf("My Parent process's id is %d\n", getppid());

	printf("exec starts\n");
        // TODO 1: Call exec with exec_overlay.exe as an argument

	//execv("exec_overlay.exe", argv_list);
	execvpe("ls", argv_list, envp);
	printf("This won't print\n");
	return 0;
}
