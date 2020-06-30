#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */

int main(int argc, char *argv[])
{
    pid_t filho;
    for (int i = 1; i <= 10; i++)
    {
        if ((filho = fork()) == 0)
        {
            printf("Identificador do processo filho através do filho %d\n", getpid());
            printf("Identificador do processo pai através do filho %d\n", getppid());
            printf("Saiu o filho %d\n", i);
            _exit(0);
        }
        else
        {
            printf("Código de saida %d\n", waitpid(filho, 0, WUNTRACED));
            printf("-----------------------------------------\n");
        }
    }
}