#include "stdafx.h"

#include "process.h"

sig_atomic_t sigusr1_count = 0;
static void handler (int signal_number)
{
    ++sigusr1_count;
}

void TestSignal()
{
    struct sigaction sa;
    memset (&sa, 0, sizeof (sa));
    sa.sa_handler = &handler;
    sigaction (SIGUSR1, &sa, NULL);

    pid_t myPid =getpid();
    /* Send SIGUSR1 signal to myself by kill() command. */
    for (unsigned int i = 0; i < 10; ++i)
        kill(myPid, SIGUSR1);

    /* Do some lengthy stuff here. */
    sleep(1);

    printf ("SIGUSR1 was raised %d times\n", sigusr1_count);
}

