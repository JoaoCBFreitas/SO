#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

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
            char input[256];
            while (strcmp(input, "exit") != 0)
            {
                scanf("%s", input);
                write(pd[1], input, strlen(input) + 1);
            }
            close(pd[1]);
        }
        else
        {
            close(pd[1]);
            dup2(pd[0], 0); //redirecionamento do STDIN para descritor de leitura
            close(pd[0]);
            char buf[1024];
            while (strcmp(buf, "exit") != 0)
            {
                read(0, buf, sizeof(buf));
                execl("/bin/wc", "wc", buf, NULL);
            }
        }
    }
}