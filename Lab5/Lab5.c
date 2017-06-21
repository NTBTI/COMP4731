#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>

#define NITEMS 20

#define NPRODUCERS 2
#define NCONSUMERS 2
struct
{
    int next_write;
    int next_read;
    int data[NITEMS];
} buf = {0, 0, 0, {0}};

sem_t available, used, lock;

void *doProducerStuff(void *tid)
{
    // int id = (intptr_t)tid;
    // printf("Hello from %d\n", id);
    // pthread_exit(NULL);
    int id = (intptr_t)tid;
    int i;
    int value = 0;

    while (1)
    {
        sem_wait(&available);
        sem_wait(&lock);

        i = buf.next_write;

        buf.data[i] = value++;

        printf("P%d: %d --> [%d]\n", id, buf.data[i], i);
        buf.next_write = (buf.next_write + 1) % NITEMS;

        sem_post(&lock);
        sem_post(&used);

        sleep(1);
    }
    pthread_exit(0);
}

void *doConsumerStuff(void *tid)
{
    int id = (intptr_t)tid;
    int i;

    // printf("Consumer %d says hello\n", id);
    // pthread_exit(NULL);

    while (1)
    {
        sem_wait(&used);
        sem_wait(&lock);
        i = buf.next_read;

        printf("C%d: %d <-- [%d]\n", id, buf.data[i], i);
        buf.next_read = (buf.next_read + 1) % NITEMS;
        sem_post(&lock);
        sem_post(&available);

        sleep(2);
    }

    pthread_exit(0);
}

int main(void)
{
    pthread_t producerThreads[NPRODUCERS];
    pthread_t consumerThreads[NCONSUMERS];
    int prodStatus, consStatus;

    sem_init(&available, 0, NITEMS);
    sem_init(&used, 0, 0);
    sem_init(&lock, 0, 1);

    for (int i = 0; i < NPRODUCERS; i++)
    {
        printf("Creating producer thread %d\n", i);
        prodStatus = pthread_create(&producerThreads[i], NULL, doProducerStuff, (void *)(intptr_t)i);
        if (prodStatus != 0)
        {
            printf("Uh oh....thread %d died\n", prodStatus);
            return -1;
        }
    }

    for (int i = 0; i < NCONSUMERS; i++)
    {
        printf("Creating consumer thread %d\n", i);
        consStatus = pthread_create(&consumerThreads[i], NULL, doConsumerStuff, (void *)(intptr_t)i);
        if (consStatus != 0)
        {
            printf("Consumer %d died\n", consStatus);
            return -1;
        }
    }
    //printf("Woot!\n");

    pthread_exit(0);
    return 1;
}