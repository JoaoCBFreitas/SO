#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>
int contador = 0;
clock_t tempo;
int elapsed;
void ender(int signum)
{
    printf("Control+C counter->%d\n", contador);
    kill(getpid(), SIGKILL);
}
void counter(int signum)
{
    contador++;
    elapsed = (clock() - tempo) * 10000 / CLOCKS_PER_SEC;
    printf("Tempo passado desde o início da execução->%d\n", elapsed);
}

int main(int argc, char *argv[])
{
    tempo = clock();
    if (signal(SIGQUIT, ender) == SIG_ERR)
    {
        perror("SIGQUIT FAILED");
    }
    if (signal(SIGINT, counter) == SIG_ERR)
    {
        perror("SIGINT FAILED");
    }
    while (1)
    {
        printf("Working.....\n");
        sleep(1);
    }
}
