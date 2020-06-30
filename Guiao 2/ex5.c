#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   /* chamadas ao sistema: defs e decls essenciais */
#include <sys/wait.h> /* chamadas wait*() e macros relacionadas */
//--------------Esta mais ou menos
void corre(int i)
{
    pid_t filho;
    if (i == 9)
    {
        printf("Identificador do processo %d -> %d\n", i, getpid());
        printf("Identificador do pai de %d -> %d\n", i, getppid());
        _exit(0);
    }
    else
    {
        i++;
        if ((filho = fork()) == 0)
        {
            corre(i);
            printf("Identificador do processo %d -> %d\n", i, getpid());
            printf("Identificador do pai de %d -> %d\n", i, getppid());
            waitpid(filho, 0, WUNTRACED);
            _exit(0);
        }
    }
}
int main(int argc, char *argv[])
{
    int i = 0;
    corre(i);
}