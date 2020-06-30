#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    pid_t filho[argc];
    for (int i = 0; i < argc; i++)
    {
        if ((filho[i] = fork()) == 0)
        {
            execlp(argv[i], "", NULL);
            _exit(0);
        }
    }
    for (int i = 0; i < argc; i++)
    {
        waitpid(filho[i], 0, WUNTRACED);
    }
    return 0;
}