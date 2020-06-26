#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_NAME "myfifo10"

static int isFileExists(const char *path)
{
    struct stat sfile; 
	
	stat(path, &sfile);
	
	
	if(S_ISFIFO(sfile.st_mode))
	{
		return 1;
	}
	return 0;
}

int main()
{
    char s[300];
    int num, fd;

    // TODO 1: Create FIFO using the system call
	if(!isFileExists(FIFO_NAME))  //check if file exists
	{
		umask(0);
		if(mknod(FIFO_NAME, S_IFIFO|0666, 0))
		{
			printf("Creating FIFO Failed\n");
			return -1;
		}
	}
	
    printf("Waiting for writers ...\n");
    // TODO 2: Open the FIFO for read only
	if((fd = open(FIFO_NAME, O_RDONLY))  < 0)
	{
		printf("Opening Read End Failed\n");
		return -1;
	}
    printf("Got a writer:\n");

    do
    {
        if ((num = read(fd, s, 300)) == -1)
        {
            perror("read");
        }
        else
        {
            s[num] = '\0';
            printf("tick: read %d bytes: \"%s\"\n", num, s);
        }
    }
    while (num > 0);

    close(fd);

    return 0;
}
