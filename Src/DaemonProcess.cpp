#include "stdafx.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <syslog.h>

#include "process.h"

void ForkAndExitParent()
{
    pid_t pid;
    pid = fork();
    if (pid < 0)
    {
        cerr << "Cant't fork()." << endl;
        exit(1);
    }
    else if (pid != 0)
    {
        exit(0);   /* this is parent process. */
    }
}

int GetPty(char *line)
{
    int p;
    struct stat stb;
    int i;
    int j;

    p = open("/dev/ptmx", 2);
    if (p > 0)
    {
        grantpt(p);
        unlockpt(p);
        strcpy(line, ptsname(p));
        return(p);
    }
    return -1;
}

void Daemonize()
{
    char ttyName[32];
    int pty = GetPty(ttyName);
    if (pty < 0)
    {
        syslog(LOG_ERR, "GetPty() failed !");
        exit(1);
    }
    else
    {
        syslog(LOG_USER, "GetPty() successed.");
    }

    /* Clear file creation mask. */
    umask(0);

    /* Become a session leader to lose controlling TTY. */
    ForkAndExitParent();
    setsid();

    /* Ensure future opens won't allocate controlling TTYs.
     SIGHUP, hangup signal, 系统对SIGHUP信号的默认处理是终止收到该信号的进程。
               所以若程序中没有捕捉该信号，当收到该信号时，进程就会退出. 该signal产生于以下情况:
     1 终端关闭时.该信号被发送到 session首进程以及作为job提交的进程(即用&符号提交的进程).
     2 session首进程退出时, 该信号被发送到该session中的前台进程组中的每一个进程
     */
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0)
    {
        cerr << "can't ignore SIGHUP." << endl;
        exit(1);
    }

    ForkAndExitParent();

    /* Change the current working directory to the /etc directory. */
    chdir("/etc");

    /* Close all open file descriptors */
    int i;
    for (i = 0; i < 3; i++)
        close(i);

    /* In child, open the child's side of the tty.  */
    if (open(ttyName, O_RDWR /*| O_NOCTTY*/) < 0)
    {
        syslog(LOG_ERR, "Could not open tty");
        exit(1);
    }
    dup(0);
    dup(0);

    tcsetpgrp(0, getpid());
}

void TestDaemon()
{
    Daemonize();

    execl("/bin/ls", "ls", (char *)0);

    /* NOT REACHED */
    syslog(LOG_ERR, "execl error");
}



