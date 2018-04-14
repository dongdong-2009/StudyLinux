/*
 * Pipe.cpp
 *
 *  Created on: 2013-9-17
 *      Author: LiuHao
 */

#include "stdafx.h"
#include "process.h"

/* Write COUNT copies of MESSAGE to STREAM, pausing for a second
between each. */
static void writer (const char* message, int count, FILE* stream)
{
    for (; count > 0; --count)
    {
        /* Write the message to the stream, and send it off immediately. */
        fprintf (stream, "%s\n", message);
        fflush (stream);
        /* Snooze a while. */
        sleep (1);
    }
}

/* Read random strings from the stream as long as possible. */
static void reader (FILE* stream)
{
    char buffer[1024];
    // Read until we hit the end of the stream. fgets reads until
    // either a newline or the end-of-file.
    while (!feof (stream)
        && !ferror (stream)
        && fgets (buffer, sizeof (buffer), stream) != NULL)
        fputs (buffer, stdout);
}

void TestPipe ()
{
    int fds[2];
    pid_t pid;
    //Create a pipe. File descriptors for the two ends of the pipe are
    //placed in fds.
    pipe (fds);
    /* Fork a child process. */
    pid = fork ();
    if (pid == (pid_t) 0)
    {
        FILE* stream;
        /* This is the child process. Close our copy of the write end of
           the file descriptor.
         */
        close (fds[1]);
        /* Convert the read file descriptor to a FILE object, and read from it. */
        stream = fdopen (fds[0], "r");
        reader (stream);
        close (fds[0]);
    }
    else
    {
        /* This is the parent process. */
        FILE* stream;
        /* Close our copy of the read end of the file descriptor. */
        close (fds[0]);
        /* Convert the write file descriptor to a FILE object, and write to it. */
        stream = fdopen (fds[1], "w");
        writer ("Hello, world.", 5, stream);
        close (fds[1]);
    }
}

void TestDup2()
{
    int fds[2];
    pid_t pid;
    /* Create a pipe. File descriptors for the two ends of the pipe are
        placed in fds. */
    pipe (fds);
    /* Fork a child process. */
    pid = fork ();
    if (pid == (pid_t) 0)
    {
        /* This is the child process. Close our copy of the write end of
        the file descriptor. */
        close (fds[1]);
        /* Connect the read end of the pipe to standard input. */
        dup2 (fds[0], STDIN_FILENO);
        /* Replace the child process with the "sort" program. */
        execlp ("sort", "sort", 0);
    }
    else
    {
        /* This is the parent process. */
        FILE* stream;
        /* Close our copy of the read end of the file descriptor. */
        close (fds[0]);
        /* Convert the write file descriptor to a FILE object, and write
        to it. */
        stream = fdopen (fds[1], "w");
        fprintf (stream, "This is a test.\n");
        fprintf (stream, "Hello, world.\n");
        fprintf (stream, "My dog has fleas.\n");
        fprintf (stream, "This program is great.\n");
        fprintf (stream, "One fish, two fish.\n");
        fflush (stream);
        close (fds[1]);
        /* Wait for the child process to finish. */
        waitpid (pid, NULL, 0);
    }
}


