#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include "sem.h"

#define SEGMENT_SIZE 0x400

int main()
{
    key_t key = 0x12345; // Using same for both shm & sem
    int segment_id;
    char *shared_memory;
    int sem_id;
	struct shmid_ds shmbuffer;

    // TODO 1: Allocate shared memory segment using shmget
	segment_id = shmget(key, SEGMENT_SIZE, IPC_CREAT| S_IRUSR| S_IWUSR);
	if(segment_id == EEXIST)
	{
		printf("The memory segment is already in use\n");
		return -1;
	}
    printf("1: Allocated a shared segment   segment id: %d (0x%X)\n", segment_id, segment_id);

    // TODO 2: Attach shared memory segment
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
	}

    printf("3: Semaphore created with id: %d\n", sem_id);
    // TODO 4: Invoke wait
	if(binary_semaphore_wait(sem_id))
	{
		printf("resouorce aquire failed\n");
		return -1;
	}
    printf("3: Wait call returned\n");

    /* Print out the string from shared memory */
    printf("4: Shared memory has \"%s\"\n", shared_memory);

    /* Write a string to the shared memory segment */
    sprintf(shared_memory, "I am fine. How r u?");
    printf("2: Message posted and timepass for a 6 secs\n");
    sleep(6);

    // TODO 5: Invoke post
	if(binary_semaphore_post(sem_id))
	{
		printf("resouorce release failed\n");
		return -1;
	}
    printf("5: Post call returned\n");

    // TODO 6: De-allocate the 
	if(binary_semaphore_deallocate(sem_id))
	{
		printf("deallocation failed failed\n");
		return -1;
	}
    printf("6: Semaphore freed\n");

    // TODO 7: Detach the shared memory
	if(shmdt(shared_memory) < 0)
	{
		printf("detach failed \n");
		return -1;
	}
    printf("7: Unmapped shared memory\n");
    sleep(2);

    // TODO 8: Deallocate the shared memory segment
	if(shmctl(segment_id, IPC_RMID, &shmbuffer) < 0)
	{
		printf("deleting the segment failed\n");
		return -1;
	}
    printf("8: Finally deallocated the shared memory\n");

    return 0;
}
