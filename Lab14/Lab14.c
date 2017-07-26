#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

#define TABSPACES 2
#define DEBUG 0

int readDir(char *dirPath, int depth)
{
    DIR *dp;
    struct dirent *dir;
    struct stat dirInfo;
    int tab = depth * TABSPACES;

    //first check the thing we were given IS a dir
    if (DEBUG)
    {
        printf("We're at depth %d\n", tab);
    }
    int fpStat = stat(dirPath, &dirInfo);
    if (fpStat < 0)
    {
        perror("stat");
        return EXIT_FAILURE;
    }
    if (!S_ISDIR(dirInfo.st_mode))
    {
        //nope
        if (DEBUG)
        {
            printf("%s is not a dir. Skipping\n", dirPath);
        }
        return 0;
    }

    if ((dp = opendir(dirPath)) == NULL)
    {
        return EXIT_FAILURE;
    }

    if ((chdir(dirPath)) == -1)
    {
        return EXIT_FAILURE;
    }

    //print the dir name itself
    printf("%*s%s/\n", tab, "", dirPath);

    //then read it
    while ((dir = readdir(dp)) != NULL)
    {
        stat(dir->d_name, &dirInfo);
        if (S_ISDIR(dirInfo.st_mode))
        {
            //ignore the two special dirs structures
            if ((strcmp(dir->d_name, ".") == 0) || (strcmp(dir->d_name, "..") == 0))
            {
                continue;
            }
            //recursion!
            readDir(dir->d_name, depth + 1);
        }
        else if (S_ISREG(dirInfo.st_mode))
        {
            //we're a file
            printf("%*s%s\n", tab + TABSPACES, "", dir->d_name);
        }
    }
    chdir("..");
    closedir(dp);
    return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Need an argument\n");
        return EXIT_FAILURE;
    }
    //just take the one arg
    //readDir(argv[1], 0);

    for (int i = 1; i < argc; i++)
    {
        readDir(argv[i], 0);
        printf("\n");
    }
}
