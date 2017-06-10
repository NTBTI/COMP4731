#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

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
    char *argv[MAXARGS];

    write(STDOUT_FILENO, "> ", 3);
    while ((n = getline(&line, &size, stdin)) != -1)
    {
        //printf("Line: %s\n", line);
        token = strtok(line, " \n"); //Why the \n here? Well getline() INCLUDES the newline, so we need to get rid of it on each strtok() call

        cmd = token;
        //printf("Token: %s\n", token);
        //printf("Command is %s\n", cmd);
        //we would go through the other tokens here if we wanted
        argv[0] = token;
        int i = 1;
        while (token != NULL)
        {
            token = strtok(NULL, " \n"); //see above :)
            //printf("Next Token --> %s\n", token);

            argv[i] = token;
            i++;
        }
        //argv[i + 1] = NULL; //not actually technically needed becuase we're checking for NULL and would have assigned one at the end of the loop

        //fork the process and run the command
        childPid = fork();
        if (childPid == -1)
        {
            perror("fork");
            return EXIT_FAILURE;
        }
        if (childPid == 0)
        {
            //we're in the child...gross...
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
