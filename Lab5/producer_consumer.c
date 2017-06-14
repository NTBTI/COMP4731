#include <semaphore.h>
#define NITEMS 100

#define NPRODUCERS 2
#define NCONSUMERS 2

struct
{
    int nused;        //number of items currently in buffer
    int next_write;   //next location to store data (by producer)
    int next_read;    //next location to retrieve data (by consumer)
    int data[NITEMS]; //actual data (the buffer)
} buf = {0, 0, 0 {0}};

//for Mac we *might* need named semaphores
sem_t s;
sem_init(&s, 0, intial_value);
sem_wait(&s);
sem_post(&s);
sem_destroy(&s);

char *semName = "/tmp/tmpsemaphore";
s = sem_open(semName, O_CREAT, 0600, initial_value);
sem_close(&s);
sem_unlink(semName);