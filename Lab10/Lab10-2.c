#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

#define DEBUG 1

//so there goes an hour...*nix uses MAP_ANONYMOUS and OS X uses MAP_ANON
#ifdef __APPLE__
#define MAP_ANONYMOUS MAP_ANON
#endif

int main(void)
{
    int *addr;
    pid_t pid;

    //set anon-access
    addr = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
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
        *addr = 90210;
        if (DEBUG)
        {
            printf("In the child process. Writing the value %d\n", *addr);
        }

        if (munmap(addr, sizeof(int)) < 0)
        {
            //failed to write to shared mem
            perror("Error writing to shared mem\n");
            return EXIT_FAILURE;
        }
    default:        //parent
        wait(&pid); //wait for the child to die.
        printf("Value from child is %d\n", *addr);

        if (munmap(addr, sizeof(int)) == -1)
        {
            perror("Could not unmap\n");
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
