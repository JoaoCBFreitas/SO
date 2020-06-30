#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */

int main(int argc, char *argv[])
{
    pid_t filho;
    if ((filho = fork()) == 0)
    {
        printf("Identificador do processo filho através do filho %d\n", getpid());
        printf("Identificador do processo pai através do filho %d\n", getppid());
    }
    else
    {
        printf("Identificador do processo filho através do pai %d\n", filho);
        printf("Identificador do processo do pai através do pai %d\n", getpid());
        printf("Identificador do processo pai do pai através do pai %d\n", getppid());
    }
}