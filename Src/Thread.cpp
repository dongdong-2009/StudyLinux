/*
 * Thread.cpp
 *
 *  Created on: 2013-9-17
 *      Author: LiuHao
 */

#include "stdafx.h"
#include "thread.h"

/* A mutex protecting job. */
static pthread_mutex_t jobMutex = PTHREAD_MUTEX_INITIALIZER;
void* TestMutexes(void*)
{
    pthread_mutex_lock (&jobMutex);

    /* do something. */
    cout << "Doing something." << endl;

    pthread_mutex_unlock (&jobMutex);

    return NULL;
}

void TestThread()
{
    pthread_t threadId;
    /* Create a new thread to run TestMutexes() function. */
    pthread_create (&threadId, NULL, &TestMutexes, NULL);
    /* Wait for all threads to finish. */
    pthread_join (threadId, NULL);
}

