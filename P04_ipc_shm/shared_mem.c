#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <errno.h>

#define KEY IPC_PRIVATE // 0x12345 // Would retrieve the same if not detached

int main()
{
    int segment_id;
    char *shared_memory;
    struct shmid_ds shmbuffer;
    int segment_size;
    const int shared_segment_size = 0x6400;

    // TODO 1: Allocate shared memory segment using shmget
	segment_id = shmget(KEY, shared_segment_size, IPC_CREAT|IPC_EXCL| S_IRUSR| S_IWUSR);
	if(segment_id == EEXIST)
	{
		printf("The memory segment is already in use\n");
		return -1;
	}
	
    printf("The segment id: %d (0x%X)\n", segment_id, segment_id);

    // TODO 2: Attach shared memory segment
	shared_memory = (char *) shmat(segment_id, NULL, 0);
	
	if(shared_memory < 0)
	{
		printf("atach failed \n");
		return -1;
	}
	
    printf("Shared memory attached at address %p\n", shared_memory);

    // TODO 3: Determine the segment's size
	if(shmctl(segment_id, IPC_STAT, &shmbuffer) < 0)
	{
		return -1;
	}
	
	segment_size = shmbuffer.shm_segsz;
    printf("Segment size: %d (0x%X) bytes\n", segment_size, segment_size);

    /* Write a string to the shared memory segment */
    sprintf(shared_memory, "Hello World");
    printf("Wrote \"Hello World\" into it\n");
    // TODO 4: Detach the shared memory segment
	if(shmdt(shared_memory) < 0)
	{
		printf("detach failed \n");
		return -1;
	}
    printf("Shared memory detached\n");

    // TODO 5: Reattach the shared memory segment, at 0x50000000
	shared_memory = (char *) shmat(segment_id, 0x50000000, 0);
	
	if(shared_memory < 0)
	{
		printf("atach failed \n");
		return -1;
	}
    printf("Shared memory reattached at address %p\n", shared_memory);
    /* Print out the string from shared memory */
    printf("It contains: %s\n", shared_memory);
    // TODO 6: Detach the shared memory segment
	if(shmdt(shared_memory) < 0)
	{
		printf("detach failed \n");
		return -1;
	}
	
    printf("Shared memory detached\n");
    // TODO 7: Deallocate the shared memory segment
	if(shmctl(segment_id, IPC_RMID, &shmbuffer) < 0)
	{
		printf("deleting the segment failed\n");
		return -1;
	}
	printf("Deallocation of shared memory sucsseful\n");
    return 0;
}
