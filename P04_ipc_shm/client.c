#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "sig.h"
#include "em_registers.h"

#define FIFO_NAME "myfifo12"

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
    int fd;
    struct em_registers reg;

    // TODO 1: Create the FIFO
	if(!isFileExists(FIFO_NAME))  //check if file exists
	{
		if(mkfifo(FIFO_NAME, S_IFIFO|0666))
		{
			printf("Creating  FIFO  Failed\n");
			return -1;
		}
	}
    printf("Waiting for writers ...\n");
    // TODO 2: Open the FIFO
	if((fd = open(FIFO_NAME, O_RDONLY))  < 0)
	{
		printf("Opening Read End Failed\n");
		return -1;
	}
    printf("Got a writer:\n");

    while ((read(fd, &reg, sizeof(struct em_registers))) > 0)
    {
        printf("Va = %u, Vb = %u Vc = %u\n", reg.va, reg.vb, reg.vc);
    }
   
    close(fd);

    return 0;
}
