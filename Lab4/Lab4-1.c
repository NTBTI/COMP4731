#include <pthread.h>
#include <stdio.h>

#define NUMLOOPS 1000

void *printTheWord(void *word)
{
    //cast to a string. We need to do this becuase we can pass ANYTHING in. In this case it happens to be a char[]
    char *theWord = (char *)word;
    printf("%s\n", theWord);
    pthread_exit(NULL);
}

int main(void)
{
    pthread_t thread1;
    pthread_t thread2;
    for (int i = 0; i < NUMLOOPS; i++)
    {
        pthread_create(&thread1, 0, printTheWord, "hello");
        pthread_create(&thread2, 0, printTheWord, "world");
    }
    pthread_exit(0);
    return 0;
}
