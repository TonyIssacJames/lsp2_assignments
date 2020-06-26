#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "sig.h"

#define FIFO_FILE "myfifo11"

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

void broken_pipe(int sno)
{
    printf("Broken Pipe %d. Let's continue ...\n", sno);
	sleep(1);
}

int main(int argc, char *argv[])
{
    int cnt = 1, i;
    FILE *fp;
	sigset_t  sa_mask;
	
	sigemptyset(&sa_mask);
	
    printf("fifoclient:%d\n",__LINE__);
    if ((argc != 2) && (argc != 3))
    {
        printf("Usage: fifoclient <string> [ <cnt> ]\n");
        return 1;
    }
    if (argc == 3)
    {
        cnt = atoi(argv[2]);
    }
	
	printf("fifoclient:%d\n",__LINE__);
    //TODO 1: Register the handler for SIGPIPE
	if(signal_register(SIGPIPE, broken_pipe, NULL, &sa_mask) )
	{
		printf("Failed: signal handler registration\n");
		return  -1;
	}
	
	printf("fifoclient:%d\n",__LINE__);
    //TODO 2: Create the FIFO using library function
	if(!isFileExists(FIFO_FILE))  //check if file exists
	{
		if(mkfifo(FIFO_FILE, S_IFIFO|0666))
		{
			printf("Creating  FIFO  Failed\n");
			return -1;
		}
	}
	
	printf("fifoclient:%d\n",__LINE__);
    printf("\nSending %d \"%s\" to %s \n", cnt, argv[1], FIFO_FILE);
    fflush(stdout);
    //TODO 3: Open the FIFO file
	 printf("Waiting for readers ...\n");
	// Open the FIFO for write only 
	if((fp = fopen(FIFO_FILE, "w"))  == NULL)
	{
		printf("Opening Write End Failed\n");
		return -1;
	}
	printf("Got a reader - Type some stuff\n");
	
     for (i = 0; i < cnt; i++)
    {
        // TODO 4: Write to the FIFO using fputs
		if(fputs(argv[1], fp) == EOF)
		{
			printf("Error Writing to pipeout_fp\n");
		}
		
        printf(".");
        fflush(stdout);
        sleep(1);
    }
    printf(" done\n");
    fclose(fp);

    return 0;
}
