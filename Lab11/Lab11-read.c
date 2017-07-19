#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

#define DEBUG 1

#define SHAREDMEMLOC "/comp4731lab11sharedmem"

int main(void)
{
    int *shmData;
    int size = sizeof(shmData);
    int shmFP;
    int numInts = 0;

    //open the shared mem location

    shmFP = shm_open(SHAREDMEMLOC, O_RDONLY, 0);

    if (shmFP < 0)
    {
        perror("shm_open");
        return (EXIT_FAILURE);
    }

    shmData = mmap(0, size, PROT_READ, MAP_SHARED, shmFP, 0);
    if (shmData == MAP_FAILED)
    {
        perror("mmap");
        return (EXIT_FAILURE);
    }

    //first location is how many integers we put in
    numInts = shmData[0];
    if (numInts < 0)
    {
        return (EXIT_FAILURE);
    }

    if (DEBUG)
    {
        printf("Expect %d integers\n", numInts);
    }

    for (int i = 1; i < numInts + 1; i++)
    {
        printf("Shared int %d is --> %d\n", i, shmData[i]);
    }

    if (munmap(shmData, size) < 0)
    {
        perror("munmap");
        return (EXIT_FAILURE);
    }
    if (shm_unlink(SHAREDMEMLOC) != 0)
    {
        perror("shm_unlink");
        return (EXIT_FAILURE);
    }
}
