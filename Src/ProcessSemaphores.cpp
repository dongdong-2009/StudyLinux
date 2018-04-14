#include "stdafx.h"
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "process.h"

/* We must define union semun ourselves. */
union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};

/* Wait on a binary semaphore. Block until the semaphore value is positive, then
 decrement it by 1. */
int binary_semaphore_wait(int semid)
{
    struct sembuf operations[1];
    /* Use the first (and only) semaphore. */
    operations[0].sem_num = 0;
    /* Decrement by 1. */
    operations[0].sem_op = -1;
    /* Permit undo¡¯ing. */
    operations[0].sem_flg = SEM_UNDO;
    return semop(semid, operations, 1);
}

/* Post to a binary semaphore: increment its value by 1.
 This returns immediately. */
int binary_semaphore_post(int semid)
{
    struct sembuf operations[1];
    /* Use the first (and only) semaphore. */
    operations[0].sem_num = 0;
    /* Increment by 1. */
    operations[0].sem_op = 1;
    /* Permit undo¡¯ing. */
    operations[0].sem_flg = SEM_UNDO;
    return semop(semid, operations, 1);
}

void TestProcessSemaphore()
{
    pid_t child_pid;
    int semID;
    int semFlags;
    semFlags = IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR
               | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

    /* Create Process Semaphore. */
    semID = semget(IPC_PRIVATE, 1, semFlags);

    /* Initialize the semaphore. */
    union semun argument;
    unsigned short values[1] = {0};
    argument.array = values;
    semctl(semID, 0, SETALL, argument);

    /* Duplicate this process. */
    child_pid = fork();
    if (child_pid != 0)
    {
        /* This is the parent process. */
        printf("I am parent process, semID=%d! \r\n", semID);

        for (int i = 0; i < 3; ++i)
        {
            sleep(1);
            binary_semaphore_post(semID);
        }

        int childStatus;
        wait(&childStatus);
        if (WIFEXITED (childStatus))
        {
            printf("the child process exited normally, with exit code %d\n",
                   WEXITSTATUS (childStatus));
        }
        else
            printf("the child process exited abnormally\n");

        union semun ignoredArgument;
        semctl(semID, 1, IPC_RMID, ignoredArgument);
    }
    else
    {
        for (int i = 0; i < 3; ++i)
        {
            binary_semaphore_wait(semID);
            printf("I am child process, semID=%d! \r\n", semID);
        }
    }
}

