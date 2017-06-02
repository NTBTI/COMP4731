#include <unistd.h>
#include <ctype.h>

#define BUFSIZE 1024

int main(void)
{
    char readBuff[BUFSIZE];
    ssize_t n;

    write(STDOUT_FILENO, "Enter something\n", 17);
    while ((n = read(STDIN_FILENO, readBuff, BUFSIZE)) > 0)
    {
        //write(STDOUT_FILENO, readBuff, n);
        for (int i = 0; i < n; i++)
        {
            readBuff[i] = toupper(readBuff[i]);
        }
        write(STDOUT_FILENO, readBuff, n);
        write(STDOUT_FILENO, "\nEnter something else\n", 24);
    }
    return 1;
}
