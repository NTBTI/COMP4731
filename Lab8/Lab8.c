#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>

#define DEBUG 0

#define PIPESIDE_LEFT 1
#define PIPESIDE_RIGHT 2

#define MAXARGS 1024

int execPipeSide(int pipeFD[], char *argv[], int pipeSide)
{
    pid_t pid;
    pid = fork();
    int fd;
    int dupChan;

    for (int i = 0; argv[i] != '\0'; i++)
    {
        printf("LEft is %s\n", argv[i]);
    }
    //see which side of the pipe we're using
    if (pipeSide == PIPESIDE_LEFT)
    {
        fd = pipeFD[1];
        dupChan = dup2(fd, STDOUT_FILENO);
    }
    else
    {
        //this is...backwards?
        fd = pipeFD[0];
        //we don't need the open fd[1] channel
        close(pipeFD[1]);
        dupChan = dup2(fd, STDIN_FILENO);
    }

    switch (pid)
    {
    case -1: //error forking
        perror("Erorr forking\n");
        exit(EXIT_FAILURE);
    case 0: //in the child
        //run the commands passed in
        if (dupChan < 0)
        {
            perror("Error duplicating std\n");
            exit(EXIT_FAILURE);
        }
        return execvp(*argv, argv);
    default: //parent
        printf("WTF?\n");
        exit(-1);
    }
}

int execNoPipe(char *argv[])
{
    pid_t pid;

    pid = fork();
    switch (pid)
    {
    case -1:
        return EXIT_FAILURE;
    case 0:
        //do the actual execution, were in the child
        return execvp(*argv, argv);
    default:
        wait(0);
    }
    return EXIT_SUCCESS;
}

int execLeftPipe(int pipeFD[], char *argv[])
{
    pid_t pid;
    pid = fork();

    switch (pid)
    {
    case -1:
        return EXIT_FAILURE;
    case 0:
        if (pipeFD[1] != STDOUT_FILENO && dup2(pipeFD[1], STDOUT_FILENO) == -1)
        {
            return EXIT_FAILURE;
        }
        if (execvp(*argv, argv) == -1)
        {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int execRightPipe(int pipeFD[], char *argv[])
{
    pid_t pid;

    pid = fork();
    switch (pid)
    {
    case -1:
        return EXIT_FAILURE;
    case 0:

        close(pipeFD[1]);

        if (pipeFD[0] != STDIN_FILENO && dup2(pipeFD[0], STDIN_FILENO) == -1)
        {
            close(pipeFD[0]);
        }

        if (execvp(*argv, argv) == -1)
        {
            return (EXIT_FAILURE);
        }
    }
    return EXIT_SUCCESS;
}

int main(void)
{
    char *line = NULL;
    size_t size;
    ssize_t n;
    pid_t childPid;
    //int pipeFD[2]; //assume one pipe

    char *token;
    char *argv[MAXARGS];
    char *leftInp, *rightInp;
    char *leftArgs[MAXARGS];
    char *rightArgs[MAXARGS];

    int pipeFD[2]; //assuming one pipe
    int status;
    int hasPipe = 0;

    write(STDOUT_FILENO, "> ", 3);
    while ((n = getline(&line, &size, stdin)) != -1)
    {
        int i = 0;
        char *tmp;
        tmp = line;
        while ((token = strtok(tmp, "|")) != NULL)
        {
            hasPipe = 1;
            argv[i++] = token;
            if (DEBUG)
            {
                printf("i: %d --> Arg: %s\n", i, token);
            }
            tmp = NULL;
        }

        //left side of pipe
        i = 0;
        leftInp = argv[0];
        //break apart the left side commands
        while ((token = strtok(leftInp, " \n")) != NULL)
        {
            leftArgs[i++] = token;
            if (DEBUG)
            {
                printf("Left cmd: %s\n", token);
            }
            leftInp = 0;
        }

        i = 0;
        rightInp = argv[1];
        //and the right
        while ((token = strtok(rightInp, " \n")) != NULL)
        {
            rightArgs[i++] = token;
            if (DEBUG)
            {
                printf("Right cmd: %s\n", token);
            }
            rightInp = 0;
        }

        pipe(pipeFD);

        if (hasPipe)
        {
            int leftStatus, rightStatus;
            leftStatus = execLeftPipe(pipeFD, leftArgs);
            rightStatus = execRightPipe(pipeFD, rightArgs);
            if (leftStatus < 0 || rightStatus < 0)
            {
                perror("Problem in one of the pipe");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            int execStatus;
            execStatus = execNoPipe(leftArgs);
            if (execStatus != 0)
            {
                perror("Problem with exec no pipe");
                exit(EXIT_FAILURE);
            }
        }

        //close the pipe
        close(pipeFD[0]);
        close(pipeFD[1]);

        //wait for the child to die...so sad...so so sad...
        while ((childPid = wait(&status)) != -1)
        {
        }
        write(STDOUT_FILENO, "> ", 3);
    }

    return EXIT_SUCCESS;
}
