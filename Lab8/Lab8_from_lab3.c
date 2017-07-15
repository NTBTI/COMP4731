#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#define DEBUG 1

#define MAXARGS 1024

//check http://www.tutorialspoint.com/unix_system_calls/pipe.htm

int main(void)
{
    char *line = NULL;
    //const char sep[2] = " ";
    size_t size;
    ssize_t n;
    //pid_t childPid;
    char *token;
    char *cmd;
    char *leftSide;
    char *rightSide;
    //char *argv[MAXARGS];

    write(STDOUT_FILENO, "> ", 3);
    while ((n = getline(&line, &size, stdin)) != -1)
    {
        //printf("Line: %s\n", line);
        token = strtok(line, "|\n"); //Why the \n here? Well getline() INCLUDES the newline, so we need to get rid of it on each strtok() call

        cmd = token;

        leftSide = &token[0];
        rightSide = &token[1];
        printf("Token: %s\n", token);
        printf("Left pipe: %s\n", leftSide);
        printf("Right pipe: %s\n", rightSide);
        //we would go through the other tokens here if we wanted
        // int i = 1;
        // while (token != NULL)
        // {
        //     token = strtok(NULL, " \n"); //see above :)
        //     printf("Next Token --> %s\n", token);

        //     argv[i] = token;
        //     i++;
        // }
        //argv[i + 1] = NULL; //not actually technically needed becuase we're checking for NULL and would have assigned one at the end of the loop

        write(STDOUT_FILENO, "> ", 3);
    }
    return EXIT_SUCCESS;
}
