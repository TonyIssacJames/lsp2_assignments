#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "sem.h"
#include <errno.h>

#define SEGMENT_SIZE 0x400

int main()
{
    key_t key = 0x12345; // Using same for both shm & sem
    int segment_id;
    char *shared_memory;
    int sem_id;

    // TODO 1: Allocate shared memory segment using shmget
	segment_id = shmget(key, SEGMENT_SIZE, IPC_CREAT| S_IRUSR| S_IWUSR);
	if(segment_id == EEXIST)
	{
		printf("The memory segment is already in use\n");
		return -1;
	}
    printf("1: Allocated a shared segment   segment id: %d (0x%X)\n", segment_id, segment_id);

    // TODO 2: Attach shared memory 
	shared_memory = (char *) shmat(segment_id, NULL, 0);
	
	if(shared_memory < 0)
	{
		printf("atach failed \n");
		return -1;
	}
	
    printf("2: Mapped shared memory at address %p\n", shared_memory);

    // TODO 3: Allocate the binary semaphore
	sem_id = binary_semaphore_allocate(key, IPC_CREAT| S_IRUSR| S_IWUSR);
	
	if(sem_id < 0)
	{
		printf("sem allocation failed \n");
		return -1;
	}

    printf("3: Semaphore created with id: %d\n", sem_id);

    // TODO 4: Initialize the semaphore
	if(binary_semaphore_set(sem_id))
	{
		printf("sem init failed  s\n");
		return -1;
	}

    printf("4: Semaphore set to 1\n");

    // TODO 5: Invoke wait
	if(binary_semaphore_wait(sem_id))
	{
		printf("resouorce aquire failed\n");
		return -1;
	}
    printf("5: Wait call returned\n");

    /* Write a string to the shared memory segment */
    sprintf(shared_memory, "Hi how r you?");
    printf("6: Message posted and timepass for half a minute. Now run ./sem_two\n");
    sleep(30);

    // TODO 6: Invoke post
	if(binary_semaphore_post(sem_id))
	{
		printf("resouorce release failed\n");
		return -1;
	}
    printf("7: Post call returned\n");

    // TODO 7: Invoke wait
	if(binary_semaphore_wait(sem_id))
	{
		printf("resouorce aquire failed\n");
		return -1;
	}
    printf("8: Wait call returned\n");	

    /* Print out the string from shared memory */
    printf("9: Shared memory has \"%s\"\n", shared_memory);

    // TODO 8: Invoke post
	if(binary_semaphore_post(sem_id))
	{
		printf("resouorce release failed\n");
		return -1;
	}
    printf("10: Post call returned\n");

    // TODO 9: Detach the shared memory
	if(shmdt(shared_memory) < 0)
	{
		printf("detach failed \n");
		return -1;
	}
    printf("11: Unmapped shared memory\n");

    return 0;
}
