#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <errno.h>

#include "sig.h"
#include "em_registers.h"
#include "sem.h"


#define FIFO_NAME "myfifo12"
#define KEY 0x12345 // Would retrieve the same if not detached
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

void sig(int signum)
{
    printf("Received signal %d\n", signum);
}

int get_lib_count()
{
    int libcount = 0;
	FILE *pipein_fp;
	
    char readbuf[80];
    // TODO 6: Open the pipe with ls lib*.so and return the lib count
     if((pipein_fp = popen("ls lib*.so", "r"))== NULL)
	{
		printf("Error opening pipein\n");
		return -1;
	}
	
	 /* Processing loop */
    while (fgets(readbuf, 80, pipein_fp) != NULL)
	{
		libcount++;
	}
   
    /* Close the pipes */
    pclose(pipein_fp);
	
    return  libcount;
}

int main()
{
    int fd, num, libcount;
    struct em_registers reg;
    int shm_id;
    char *shm_addr = NULL;
    const int shm_size = sizeof(struct em_registers);
    int sem_id;

	sigset_t  sa_mask;
	
	sigemptyset(&sa_mask);
	
    memset(&reg, 0, sizeof(struct em_registers));
    reg.va = 440;
    reg.vb = 438;
    reg.vc = 430;
    
    // TODO 1: Register handler sig for SIGINT and SIGPIPE
	if(signal_register(SIGPIPE, sig, NULL, &sa_mask) )
	{
		printf("Failed: signal handler registration\n");
		return  -1;
	}
	
	if(signal_register(SIGINT, sig, NULL, &sa_mask) )
	{
		printf("Failed: signal handler registration\n");
		return  -1;
	}

    // TODO 2: Get the count of lib*.so files using pipe
	if((libcount = get_lib_count())  < 0)
	{
		printf("Error in get_lib_count\n");
	}
    if (libcount > 0)
    {
        printf("Library count = %d\n", libcount);
    }

    // TODO 9: Allocate the shared memory
	shm_id = shmget(KEY, shm_size, IPC_CREAT| S_IRUSR| S_IWUSR);
	if(shm_id == EEXIST)
	{
		printf("The memory segment is already in use\n");
		return -1;
	}
    printf("9: Allocated a shared segment   segment id: %d (0x%X)\n", shm_id, shm_id);
	shm_addr = (char *) shmat(shm_id, NULL, 0);
    printf("Shared memory attached at address %p\n", shm_addr);

    // TODO 10: Allocate the binary semaphore
	sem_id = binary_semaphore_allocate(KEY, IPC_CREAT| S_IRUSR| S_IWUSR);
	
	if(sem_id < 0)
	{
		printf("sem allocation failed \n");
	}
    printf("Semaphore created with id: %d\n", sem_id);

	/*
    // TODO 11: Initialize the binary semaphore
	if(binary_semaphore_set(sem_id))
	{
		printf("sem init failed  s\n");
		return -1;
	}

    printf("11: Semaphore set to 1\n");
	*/
    
    // TODO 3: Create the FIFO
	if(!isFileExists(FIFO_NAME))  //check if file exists
	{
		if(mkfifo(FIFO_NAME, S_IFIFO|0666))
		{
			printf("Creating  FIFO  Failed\n");
			return -1;
		}
	}
    printf("Waiting for readers ...\n");
    // TODO 4: Open the FIFO
    if((fd = open(FIFO_NAME, O_WRONLY))  == NULL)
	{
		printf("Opening Write End Failed\n");
		return -1;
	}
       printf("Got a reader - Sending registers\n");

    // TODO 12: Get the semaphore
    while (1) 
    {
        // TODO 13: Synchronize the access to shared memory using semaphore
		/* Wait call */
		binary_semaphore_wait(sem_id);
        memcpy(&reg, shm_addr, sizeof(struct em_registers)); 
        /* Post call */
        binary_semaphore_post(sem_id);
        // TODO 5: Write EM Registers to the FIFO
		if ((num = write(fd, &reg,  sizeof(struct em_registers)) ) == -1)
		{
			perror("write");
			return -1;
		}        
        sleep(5);
        printf("Sent shared registers\n");
    }
       close(fd);

    return 0;
}
