#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>
//Falta por isto a fechar quando um filho encontra a palavra, não sei como é que se sabe se o filho encontrou
//Para o 3 tinha de usar sigAlarm
void childHandler(int signum)
{
    printf("Filho terminou\n");
}
int main(int argc, char *argv[])
{
    int status = 0;
    if (argc < 3)
    {
        printf("Input inválido\n");
    }
    pid_t pid[argc - 2];
    if (signal(SIGCHLD, childHandler) == SIG_ERR)
    {
        perror("SIGINT FAILED");
    }
    for (int i = 2; i < argc; i++)
    {
        if ((pid[i] = fork()) == 0)
        {
            printf("Filho %d á procura de %s em %s\n", i - 1, argv[1], argv[i]);
            execl("/bin/grep", argv[0], "-q", argv[1], argv[i], NULL);
            _exit(0);
        }
    }
    while (status == 0)
    {
        printf("Working.....\n");
        sleep(1);
    }
}