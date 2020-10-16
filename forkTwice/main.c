#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
/*
fork两次就可以产生一个子进程其父进程是init，原来的进程就无需等待子进程结束，也不会产生僵死进程
*/

int main()
{
    pid_t pid;

    if ((pid = fork()) < 0)
    {
        printf("fork error\n");
    }
    else if (pid == 0)
    {
        if ((pid = fork()) < 0)
        {
            printf("fork error\n");
        }
        else if (pid > 0)
        {
            printf("first child,parent pid=%d\n", getppid());
            return 0;
        }
        else
        {
            sleep(2);
            printf("second child,parent pid=%d\n", getppid());
            return 0;
        }
    }
    else
    {
        if (waitpid(pid, NULL, 0) != pid)
        {
            printf("waitpid error\n");
        }
        sleep(4);
        printf("parent %d end\n", getpid());
    }
}