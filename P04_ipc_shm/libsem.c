#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

union semun {
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO
							   (Linux-specific) */
};

// TODO 1: Obtain a binary semaphore id, allocating if necessary
int binary_semaphore_allocate(key_t key, int sem_flags)
{
	int mem_id;
	mem_id = semget(key, 1, sem_flags);
	
	if(mem_id == EEXIST)
	{
		if(sem_flags^IPC_EXCL)
		{
			sem_flags^=IPC_EXCL;
		}
		mem_id = semget(key, 1, sem_flags);
		if(mem_id < 0)
		{
			return -1;
		}
		return mem_id;
	}

    return mem_id;
}

// TODO 2: Deallocate a binary semaphore
int binary_semaphore_deallocate(int semid)
{
	if(semctl(semid, 1, IPC_RMID) < 0)
	{
		return -1;
	}

    return 0;
}

// TODO 3: Obtain a pre-existing binary semaphore
int binary_semaphore_get(key_t key, int sem_flags)
{
	int mem_id;

  	mem_id = semget(key, 1, sem_flags^IPC_EXCL); //create a new semaphore if nothing exists
	
	if(mem_id == EEXIST)
	{
		return -1;
	}
    return mem_id;
}

// TODO 4: Initialize a binary semaphore with a value of 1
int binary_semaphore_set(int semid)
{
	union semun attributes;
	int  errno;
	
	attributes.val = 1;
	
	errno=semctl(semid, 0, SETVAL,  attributes);
	
	if(errno< 0)
	{
		return -1;
	}
    return 0;
}

// TODO 5: Wait for the binary semaphore. Decrement the value by -1
int binary_semaphore_wait(int semid)
{
    struct sembuf operations[1];

    /* Use the first (and only) semaphore. */
    /* Decrement by 1. */
    /* Permit undoing. */
    /* Perform the operation with semop*/
	
	struct sembuf sem_op_buf[1];
	
	sem_op_buf[0].sem_num = 0;
	sem_op_buf[0].sem_op    = -1;
	sem_op_buf[0].sem_flg    = SEM_UNDO; //automatically wait when the process exits
	
	if(semop(semid, sem_op_buf,  1) < 0)
	{
		return -1;
	}
    return 0;

}

// TODO 6: Post to a binary semaphore: Increment its value by 1
int binary_semaphore_post(int semid)
{
    struct sembuf operations[1];

    /* Use the first (and only) semaphore. */
    /* Increment by 1. */
    /* Permit undoing. */
    /* Perform the operation with semop*/
	
		struct sembuf sem_op_buf[1];
	
	sem_op_buf[0].sem_num = 0;
	sem_op_buf[0].sem_op    = 1;
	sem_op_buf[0].sem_flg    = SEM_UNDO; //automatically wait when the process exits
	
	if(semop(semid, sem_op_buf,  1) < 0)
	{
		return -1;
	}
    return 0;
	
}
