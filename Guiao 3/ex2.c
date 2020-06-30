#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    pid_t filho;
    if ((filho = fork()) == 0)
    {
        execl("/bin/ls", "-l", NULL);
        _exit(0);
    }
    else
    {
        waitpid(filho, 0, WUNTRACED);
    }
    return 0;
}