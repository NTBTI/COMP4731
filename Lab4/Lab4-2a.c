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

    fp = open(fifoPath, O_RDONLY);
    //check the file opened
    if (fp < 0)
    {
        //uh oh....
        printf("Could not open the FIFO named %s\n", fifoPath);
        pthread_exit(&fp);
    }

    while (1)
    {
        if (read(fp, buffer, BUFSIZE))
        {
            printf("%s", buffer);
        }
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
