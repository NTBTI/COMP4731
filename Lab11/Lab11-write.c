#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

#define DEBUG 1

#define MAXINTS 100
#define SHAREDMEMLOC "/comp4731lab11sharedmem"

char line[10];
int input[MAXINTS + 1]; //enough for all 100 ints plus the first for the number of ints
int count = 1;          //need the first one for this

int main(void)
{
    int *shmData;
    int size = sizeof(shmData);
    int shmFP;

    if (DEBUG)
    {
        printf("Shmsize: %d\n", size);
    }
    printf("Enter a value: ");
    // fgets(line, sizeof(line), stdin);
    // sscanf(line, "%d", &value);
    while ((NULL != fgets(line, sizeof(line), stdin)) && (line[0] != '\n'))
    {
        if (1 != sscanf(line, "%d", &input[count]))
        {
            printf("Input not an int\n");
            continue;
        }
        else
        {
            if (DEBUG)
            {
                printf("Val ==> %d\n", input[count]);
            }
        }
        if (++count >= MAXINTS)
        {
            break;
        }
    }
    input[0] = count - 1;

    if (DEBUG)
    {
        for (int j = 0; j < count; j++)
        {
            printf("Value --> %d\n", input[j]);
        }
    }

    //if there are any integers entered start the shared mem
    shmFP = shm_open(SHAREDMEMLOC, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | O_TRUNC); //create and truncate if exists
    //shmFP = shm_open(SHAREDMEMLOC, O_CREAT | O_EXCL | O_RDWR, S_IRWXU | S_IRWXG); //create and truncate if exists
    if (shmFP == 0)
    {
        perror("Error creating shred mem\n");
        return (EXIT_FAILURE);
    }

    if (ftruncate(shmFP, size) == 1)
    {
        perror("Error truncating shared mem\n");
        return (EXIT_FAILURE);
    }

    shmData = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, shmFP, 0);
    if (shmData == MAP_FAILED)
    {
        perror("Error creating map\n");
        return (EXIT_FAILURE);
    }

    for (int i = 0; i < count; i++)
    {
        shmData[i] = input[i];
    }
    if (munmap(shmData, size) < 0)
    {
        perror("Error unmapping\n");
        return (EXIT_FAILURE);
    }

    if (DEBUG)
    {
        printf("Saved %d integers\n", count - 1);
    }
}
