#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */

int main(int argc, char *argv[])
{
    pid_t filho[11];
    for (int i = 1; i <= 10; i++)
    {
        if ((filho[i] = fork()) == 0)
        {
            printf("Identificador do processo filho através do filho %d\n", getpid());
            printf("Saiu o filho %d\n", i);
            _exit(0);
        }
    }
    for (int j = 1; j <= 10; j++)
    {
        waitpid(filho[j], 0, WUNTRACED);
    }
    printf("todos os filhos acabaram\n");
    for (int j = 1; j <= 10; j++)
    {
        printf("Código de saida %d\n", filho[j]);
    }
}