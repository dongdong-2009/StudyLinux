/*
 * Semaphore.cpp
 *
 *  Created on: 2013-9-18
 *      Author: LiuHao
 */

#include "stdafx.h"
#include "thread.h"

/* A mutex protecting job. */
static pthread_mutex_t jobMutex = PTHREAD_MUTEX_INITIALIZER;
static sem_t jobCount;

static void* JobThreadFunction(void*)
{
    sem_wait (&jobCount);
    pthread_mutex_lock (&jobMutex);
    /* do something. */
    cout << "Get job from queue." << endl;
    pthread_mutex_unlock (&jobMutex);
}

extern "C" void TestSemaphore()
{
    sem_init(&jobCount, 0, 0);

    pthread_t threadId;
    /* Create a new thread to run TestMutexes() function. */
    pthread_create (&threadId, NULL, &JobThreadFunction, NULL);

    pthread_mutex_lock (&jobMutex);
    cout << "Place the new job at the head of the queue" << endl;
    /* Post to the semaphore to indicate that another job is available. */
    sem_post (&jobCount);

    pthread_mutex_unlock (&jobMutex);

    /* Wait for all threads to finish. */
    pthread_join (threadId, NULL);
}


