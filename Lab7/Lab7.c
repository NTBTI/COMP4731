#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>

#define DEBUG 0

#define MAXARGS 1024

int main(void)
{
    char *line = NULL;
    //const char sep[2] = " ";
    size_t size;
    ssize_t n;
    pid_t childPid;
    char *token;
    char *cmd;
    char *outFD = NULL;
    char *inFD = NULL;
    char *argv[MAXARGS];

    write(STDOUT_FILENO, "> ", 3);
    while ((n = getline(&line, &size, stdin)) != -1)
    {
        //printf("Line: %s\n", line);
        token = strtok(line, " \n"); //Why the \n here? Well getline() INCLUDES the newline, so we need to get rid of it on each strtok() call

        //these need to be reset here INSIDE the loop. Becuase the first time you run these they are NULL
        //BUT! Every subsequent one will be stuck. So make sure to set these NULL for everything to go.
        inFD = NULL;
        outFD = NULL;

        cmd = token;
        argv[0] = token;
        int i = 1;
        while (token != NULL)
        {
            token = strtok(NULL, " \n"); //see above :)
            if (DEBUG == 1)
            {
                printf("Next Token --> %s\n", token);
            }

            if (token != NULL)
            {
                if (token[0] == '>')
                {
                    outFD = token + 1;
                    if (DEBUG == 1)
                    {
                        printf("\nYAY OUTBIUND\n");
                    }
                    continue;
                }
                if (token[0] == '<')
                {
                    inFD = token + 1;
                    if (DEBUG)
                    {
                        printf("\nYAY INBOUND!\n");
                    }
                    continue;
                }
            }

            argv[i] = token;
            i++;
        }

        //just for some debugging to see if these are set
        if (DEBUG == 1)
        {
            if (inFD != NULL)
            {
                printf("InFD --> %s\n", inFD);
                //read from this descriptor
            }

            if (outFD != NULL)
            {
                printf("OutFD --> %s\n", outFD);
            }
        }
        //fork the process and run the command
        childPid = fork();
        if (childPid == -1)
        {
            perror("fork");
            return EXIT_FAILURE;
        }

        //https://stackoverflow.com/a/14543484 for proper dup2() usage
        if (childPid == 0)
        {
            //we're in the child...gross...

            //if we have an INP and OUT
            if (inFD)
            {
                int fd1 = open(inFD, O_RDONLY, 0660);
                if (fd1 < 0)
                {
                    perror("Problem opening infd");
                    return EXIT_FAILURE;
                }
                dup2(fd1, STDIN_FILENO);
            }

            if (outFD)
            {
                int fd2 = open(outFD, O_CREAT | O_WRONLY | O_TRUNC, 0660);
                if (fd2 < 0)
                {
                    perror("Problem opening outfd");
                    return EXIT_FAILURE;
                }
                dup2(fd2, STDOUT_FILENO);
            }

            int execErr;
            execErr = execvp(cmd, argv);

            if (execErr == -1)
            {
                perror("execlp");
                return EXIT_FAILURE;
            }
        }
        else
        {
            wait(0);
        }
        write(STDOUT_FILENO, "> ", 3);
    }
    return EXIT_SUCCESS;
}
