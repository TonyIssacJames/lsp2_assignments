#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_FILE "myfifo11"

static int isFileExists(const char *path)
{
    struct stat sfile; 
	
	stat(path, &sfile);
	
	if(S_ISFIFO(sfile.st_mode))
	{
		//printf("FIFO exists\n");
		return 1;
	}
	
	//printf("FIFO don't exists\n");
	return 0;
}

int main(void)
{
	FILE *fp;
	char readbuf[80];

     //TODO 1: Create the FIFO using library function
	if(!isFileExists(FIFO_FILE))  //check if file exists
	{
		if(mkfifo(FIFO_FILE, S_IFIFO|0666))
		{
			printf("Creating  FIFO  Failed\n");
			return -1;
		}
	}
	printf("fifoserver:%d\n",__LINE__);
	printf("\nWaiting for writer clients ...\n");
	//TODO 2: Open the FIFO file
	if((fp = fopen(FIFO_FILE, "r"))  == NULL)
	{
		printf("Opening Read End Failed\n");
		return -1;
	}
	
	printf("fifoserver:%d\n",__LINE__);
	//TODO 3: Read & print until EOF
    while(fgets(readbuf, 80, fp) != NULL)
    {
		puts(readbuf);
    }
	printf("fifoserver:%d\n",__LINE__);
	
	fclose(fp);

	return 0;
}
