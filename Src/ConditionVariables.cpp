/*
 * RaceCondition.cpp
 *
 *  Created on: 2013-9-18
 *      Author: LiuHao
 */

#include "stdafx.h"
#include "thread.h"

static int threadFlag;
static pthread_mutex_t threadFlagMutex;
static pthread_cond_t threadFlagCv;

static void InitializeFlag ()
{
    /* Initialize the mutex and condition variable. */
    pthread_mutex_init (&threadFlagMutex, NULL);
    pthread_cond_init (&threadFlagCv, NULL);
    /* Initialize the flag value. */
    threadFlag = 0;
}

static void* JobThreadFunction(void*)
{
    pthread_mutex_lock (&threadFlagMutex);

    while (threadFlag == 0)
    {
        /* The flag is clear. Wait for a signal on the condition
        variable which indicating that the flag value has changed. When the
        signal arrives and this thread unblocks, loop and check the
        flag again.
        LiuHao: pthread_cond_wait() atomically unlocks the mutex and waits
        on the condition variable.
        */
        pthread_cond_wait (&threadFlagCv, &threadFlagMutex);
    }
    /* When we¡¯ve gotten here, we know the flag must be set. Unlock
    the mutex. */
    threadFlag = 0;
    pthread_mutex_unlock (&threadFlagMutex);
    /* Do some work. */
    cout << "Do some work." << endl;

    return NULL;
}

/* Sets the value of the thread flag to FLAG_VALUE. */
static void SetThreadFlag (int flagValue)
{
    /* Lock the mutex before accessing the flag value. */
    pthread_mutex_lock (&threadFlagMutex);
    /* Set the flag value, and then signal in case thread_function is
       blocked, waiting for the flag to become set. However,
       thread_function can¡¯t actually check the flag until the mutex is
       unlocked. */
    threadFlag = flagValue;
    pthread_cond_signal (&threadFlagCv);
    /* Unlock the mutex. */
    pthread_mutex_unlock (&threadFlagMutex);
}

void TestConditionVariable()
{
    pthread_t threadId;
    InitializeFlag();

    /* Create a new thread to run JobThreadFunction() function. */
    pthread_create (&threadId, NULL, &JobThreadFunction, NULL);
    SetThreadFlag(1);

    /* Wait for all threads to finish. */
    pthread_join (threadId, NULL);
}

