#include <unistd.h>

#define BUFSIZE 2014

int main(void)
{
    char readArr[BUFSIZE];
    ssize_t totalRead = 0;
    totalRead = read(STDIN_FILENO, readArr, BUFSIZE);
    write(STDOUT_FILENO, readArr, totalRead);
    return 1;
}
