#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdio.h>

#define BUFSIZE 1024

int main(int argc, char *argv[])
{
    pid_t pid;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return 1;
    }

    //good to go!
    if (pid == 0)
    {
        //gross...I would rather do this with a single call but so be it
        int execErr;
        if (argc <= 1)
        {
            execErr = execlp("ls", "ls", "-l", ".", (char *)0);
        }
        else
        {
            execErr = execlp("ls", "ls", "-l", argv[1], (char *)0);
        }

        if (execErr == -1)
        {
            perror("execlp");
            return 2;
        }
    }
    else
    {
        //remember to include the sys/wait.h as this dies in C99
        wait(0);
    }

    return 0;
}
