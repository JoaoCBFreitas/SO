#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int pd[2]; //0->leitura 1->escrita
    pipe(pd);
    pid_t p;
    if (pipe(pd) == -1)
    {
        printf("Pipe Failed\n");
        return 1;
    }
    p = fork();
    if (p < 0)
    {
        printf("Fork failed\n");
        return 1;
    }
    else
    {
        if (p > 0)
        {
            close(pd[0]);
            sleep(5);
            write(pd[1], "Input teste", 12);
            close(pd[1]);
        }
        else
        {
            close(pd[1]);
            char buf[20];
            read(pd[0], buf, sizeof(buf));
            printf("%s\n", buf);
            close(pd[0]);
        }
    }
}