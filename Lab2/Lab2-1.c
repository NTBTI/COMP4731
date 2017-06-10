#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFSIZE 1024

int main(int argc, char *argv[])
{
    char readArr[BUFSIZE];
    ssize_t readIn, writeOut;
    int inFp, outFp;

    //check we have at least 3 args
    if (argc != 3)
    {
        write(STDOUT_FILENO, "Usage: lab1-3 infile outfile\n", 30);
        return 0;
    }

    //next check the incoming file exists
    inFp = open(argv[1], O_RDONLY);
    if (inFp < 0)
    {
        write(STDERR_FILENO, "Incoming file does not exist\n", 31);
        return 0;
    }

    //next open the file to write-out
    //outFp = creat(argv[2], 0660);
    outFp = open(argv[2], O_CREAT | O_WRONLY | O_EXCL, 0660);
    if (outFp < 0)
    {
        perror("outfp");
        write(STDERR_FILENO, "Outgoing file cannot be written\n", 34);
        return 0;
    }

    while ((readIn = read(inFp, readArr, BUFSIZE)) > 0)
    {
        writeOut = write(outFp, readArr, readIn);
        if (writeOut != readIn)
        {
            perror("Outfp");
            write(STDERR_FILENO, "Error writing to file\n", 24);
            return 0;
        }
    }

    close(outFp);
    close(inFp);

    return 1;
}
