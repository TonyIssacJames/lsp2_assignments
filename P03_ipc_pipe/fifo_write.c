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
    int fd, ret, num;

    // TODO 1: Create FIFO using the system call
	if(!isFileExists(FIFO_NAME))  //check if file exists
	{
		umask(0);
		if(mknod(FIFO_NAME, S_IFIFO|0666, 0))
		{
			printf("Creating  FIFO  Failed\n");
			return -1;
		}
	}

    printf("Waiting for readers ...\n");
    // TODO 2: Open the FIFO for write only 
	if((fd = open(FIFO_NAME, O_WRONLY))  < 0)
	{
		printf("Opening Write End Failed\n");
		return -1;
	}
    printf("Got a reader - Type some stuff\n");

    while ((ret = scanf("%[^\n]%*c", s)), !feof(stdin)) 
    {
        if (ret == 0) // No input read - remove the newline
        {
            getchar();
            continue;
        }
        if ((num = write(fd, s, strlen(s))) == -1)
        {
            perror("write");
        }
        else
        {
            printf("speak: wrote %d bytes\n", num);
        }
    }

    close(fd);

    return 0;
}
