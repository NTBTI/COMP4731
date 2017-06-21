#include <unistd.h>
#include <fcntl.h>

int main(void)
{
    //here we duplicate 1 which is output. So here we write to 2, which is the same as writing to 1
    //so if you run with inclass.out > xyz it will send the output to the new file "xyz"
    dup2(1, 2);
    write(2, "Hello\n", 6);
    return 0;
}

void other(void)
{
    int fd = open("xxx", O_CREAT | O_WRONLY | O_TRUNC, 0660);
    dup2(fd, 1);
    //so here we have duplicated the file descriptor but NOT 2. So "hello" woill be written to file, but not "world"
    write(1, "Hello\n", 6);
    write(2, "world\n", 6);
}