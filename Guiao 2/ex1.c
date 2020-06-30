#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */

int main(int argc, char *argv[])
{
    pid_t filho;
    if ((filho = fork()) == 0)
    {
        printf("Identificador de processo filho %d\n", getpid());
        printf("Identificador de processo pai %d\n", getppid());
    }
}