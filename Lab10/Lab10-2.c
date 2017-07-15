#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>

#define DEBUG 1

#define MAXBUF 1024

//so there goes an hour...*nix uses MAP_ANONYMOUS and OS X uses MAP_ANON
#ifdef __APPLE__
#define MAP_ANONYMOUS MAP_ANON
#endif

int main(int argc, char **argv)
{
    //make sure we have something
    if (argc < 2)
    {
        printf("Need to give something to store\n");
        exit(EXIT_FAILURE);
    }

    //int *addr;
    char *addr;
    pid_t pid;
    size_t textLen;

    const char *textToStore = argv[1];
    textLen = strlen(textToStore) + 1; //Stupid NULL terminator...

    //set anon-access
    addr = mmap(0, textLen, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED)
    {
        perror("Mapping failed\n");
        return EXIT_FAILURE;
    }

    //create a child
    pid = fork();

    switch (pid)
    {
    case -1: //well...thats bad
        printf("Error forking\n");
        return EXIT_FAILURE;
    case 0: //Child
        for (size_t i = 0; i < textLen; i++)
        {
            addr[i] = textToStore[i];
        }

        if (DEBUG)
        {
            printf("In the child process. Writing the value %s\n", addr);
        }

        if (munmap(addr, textLen) < 0)
        {
            //failed to write to shared mem
            perror("Error writing to shared mem\n");
            return EXIT_FAILURE;
        }
    default:        //parent
        wait(&pid); //wait for the child to die.
        printf("Value from child is '%s'\n", addr);

        if (munmap(addr, textLen) == -1)
        {
            perror("Could not unmap\n");
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
