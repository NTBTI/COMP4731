#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_ITEMS 10
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2

struct
{
    int numItems;
    int nextWrite;
    int nextRead;
    int data[NUM_ITEMS];
} buf = {0, 0, 0, {0}};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t notFull = PTHREAD_COND_INITIALIZER;
pthread_cond_t notEmpty = PTHREAD_COND_INITIALIZER;

void *doConsumerStuff(void *tid)
{
    int id = (intptr_t)tid;
    int i;

    while (1)
    {
        pthread_mutex_lock(&mutex);
        //see if there is anything in the buffer
        while (buf.numItems == 0)
        {
            printf("Consumer %d is waiting for something to show up in the buffer\n", id);
            pthread_cond_wait(&notEmpty, &mutex);
        }
        i = buf.nextRead;
        buf.numItems--;

        printf("C%d: %d <-- [%d]\n", id, buf.data[i], i);
        buf.nextRead = (buf.nextRead + 1) % NUM_ITEMS;

        pthread_cond_signal(&notFull);
        pthread_mutex_unlock(&mutex);

        sleep(2);
    }

    pthread_exit(0);
}

void *doProducerStuff(void *tid)
{
    int id = (intptr_t)tid;
    int i;
    int value = 0;

    while (1)
    {
        //so we wait...and watch the buffer is not full
        while (buf.numItems == NUM_ITEMS)
        {
            //buffer is full...no room to produce, sleep
            printf("Producer %d is waiting for room in the buffer\n", id);
            pthread_cond_wait(&notFull, &mutex);
        }

        //there is room in the buffer, so put something in
        i = buf.nextWrite;
        buf.data[i] = value++;
        buf.numItems++;

        printf("P%d: %d --> [%d]\n", id, buf.data[i], i);
        buf.nextWrite = (buf.nextWrite + 1) % NUM_ITEMS;

        //now tell the consumer there is something in the buffer to consume
        //pthread_cond_signal(&notFull);
        pthread_cond_signal(&notEmpty);
        //and release the lock
        pthread_mutex_unlock(&mutex);

        sleep(1);
    }
    pthread_exit(0);
}

int main(void)
{
    pthread_t producerThreads[NUM_PRODUCERS];
    pthread_t consumerThreads[NUM_CONSUMERS];
    int prodStatus, consStatus;

    for (int i = 0; i < NUM_PRODUCERS; i++)
    {
        printf("Creating producer thread %d\n", i);
        prodStatus = pthread_create(&producerThreads[i], NULL, doProducerStuff, (void *)(intptr_t)i);
        if (prodStatus != 0)
        {
            printf("Uh oh....thread %d died\n", prodStatus);
            return -1;
        }
    }

    for (int i = 0; i < NUM_CONSUMERS; i++)
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