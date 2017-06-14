#include <pthread.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define BUFSIZE 20

void *readFIFO(void *arg)
{
    int fp;
    //cast to a string again
    char *fifoPath = (char *)arg;
    char buffer[BUFSIZE];
    ssize_t bytesRead; //set this instead so we don't use printf()

    fp = open(fifoPath, O_RDONLY);
    //check the file opened
    if (fp < 0)
    {
        //uh oh....
        printf("Could not open the FIFO named %s\n", fifoPath);
        pthread_exit(&fp);
    }

    while ((bytesRead = read(fp, buffer, BUFSIZE)) > 0)
    {
        //printf("%s", buffer); //print continues to read, but it may or may NOT contain a null term at the end, so use write() instead
        write(STDOUT_FILENO, buffer, bytesRead);
    }

    pthread_exit(0);
}

int main(void)
{
    pthread_t thread1;
    pthread_t thread2;

    pthread_create(&thread1, 0, readFIFO, "./fifo1");
    pthread_create(&thread2, 0, readFIFO, "./fifo2");

    pthread_exit(0);

    return 0;
}
