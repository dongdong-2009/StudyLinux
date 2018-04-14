#include "stdafx.h"

#include "process.h"

int Spawn ()
{
    pid_t child_pid;
    const char* program = "ls";

    /* The argument list to pass to the "ls" command. */
    const char* arg_list[] =
    {
        "ls", /* argv[0], the name of the program. */
        "-l",
        "/",
        NULL /* The argument list must end with a NULL. */
    };

    /* Duplicate this process. */
    child_pid = fork ();
    if (child_pid == 0)
    {
        /* Now execute PROGRAM, searching for it in the path. */
        execvp (program, (char**)arg_list);
        /* The execvp function returns only if an error occurs. */
        fprintf (stderr, "an error occurred in execvp\n");
        abort ();
    }

    /* This is the parent process. */
    return child_pid;
}

void TestProcess1()
{
    cout << "The process ID is " << getpid() << "!" << endl;
    cout << "The parent process ID is " << getppid() << "!" << endl;
}

void TestProcess2()
{
    /* Spawn a child process running the "ls" command. Ignore the
       returned child process ID. */
    Spawn ();
    printf ("done with main program\n");
}

void TestProcess3()
{
    int child_status = 0;
    /* The argument list to pass to the "ls" command. */
    const char* arg_list[] =
    {
        "ls", /* argv[0], the name of the program. */
        "-l",
        "/",
        NULL /* The argument list must end with a NULL. */
    };
    /* Spawn a child process running the "ls" command. Ignore the
       returned child process ID. */
    Spawn ();

    /* Wait for the child process to complete. */
    wait (&child_status);
    if (WIFEXITED (child_status))
    {
        printf ("the child process exited normally, with exit code %d\n",
                WEXITSTATUS (child_status));
    }
    else
        printf ("the child process exited abnormally\n");
}

static sig_atomic_t child_exit_status;
static void clean_up_child_process (int signal_number)
{
    /* Clean up the child process. */
    int status;
    wait (&status);
    /* Store its exit status in a global variable. */
    child_exit_status = status;
}

void TestProcess4()
{
    /* Handle SIGCHLD by calling clean_up_child_process. */
    struct sigaction sigchld_action;

    /* The argument list to pass to the "ls" command. */
    const char* arg_list[] =
    {
        "ls", /* argv[0], the name of the program. */
        "-l",
        "/",
        NULL /* The argument list must end with a NULL. */
    };
    /* Spawn a child process running the "ls" command. Ignore the
       returned child process ID. */
    Spawn ();

    memset (&sigchld_action, 0, sizeof (sigchld_action));
    sigchld_action.sa_handler = &clean_up_child_process;
    sigaction (SIGCHLD, &sigchld_action, NULL);

    sleep(1);
    if (child_exit_status)
    {
        printf ("the child process exited normally, with exit code %d\n",
                WEXITSTATUS (child_exit_status));
    }
    else
        printf ("the child process exited abnormally\n");

}


