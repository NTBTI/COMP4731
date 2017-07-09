#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

#define DEBUG 1

int main(int argc, char **argv)
{
    int fd;
    int length;
    char *addr;
    ssize_t s;

    if (argc < 3)
    {
        printf("We need a file and the number of chars to uppercase\n");
        return EXIT_FAILURE;
    }

    //check if the file opens
    fd = open(argv[1], O_RDWR);
    if (fd == -1)
    {
        perror("Cannot open file\n");
        return EXIT_FAILURE;
    }

    //read into mmap
    length = atoi(argv[2]);
    addr = mmap(0, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    //check it worked
    if (addr == MAP_FAILED)
    {
        perror("Could not map file\n");
        return EXIT_FAILURE;
    }
    //map worked!
    if (DEBUG)
    {
        s = write(STDOUT_FILENO, addr, length);
    }

    for (int i = 0; i < length; i++)
    {
        addr[i] = toupper(addr[i]);
    }

    if (DEBUG)
    {
        s = write(STDOUT_FILENO, addr, length);
    }

    if (munmap(addr, length) < 0)
    {
        perror("Could not unmap\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
