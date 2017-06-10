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

    //do our check for argc here
    if (argc <= 1)
    {
        return 1;
    }

    //good to go!
    if (pid == 0)
    {
        //setup a new char array with our command as the first el and then add from argv after that
        char *argz[BUFSIZE];
        argz[0] = "ls";
        argz[1] = "-l";
        argz[2] = "--"; //end of switches. So if we put a file of "-l"

        //gross...I would rather do this with a single call but so be it
        int execErr;
        int i;
        for (i = 1; i < argc; i++)
        {
            argz[i + 2] = argv[i];
        }
        argz[i + 2] = NULL;

        execErr = execvp("/bin/ls", argz);

        if (execErr == -1)
        {
            perror("execvp");
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
