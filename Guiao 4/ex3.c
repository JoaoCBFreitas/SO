#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    pid_t filho;
    int passw = open("passwd.txt", O_CREAT | O_TRUNC | O_WRONLY, 0640);
    int out = open("saida.txt", O_CREAT | O_TRUNC | O_WRONLY, 0640);
    int err = open("erros.txt", O_CREAT | O_TRUNC | O_WRONLY, 0640);
    dup2(passw, 0);
    dup2(out, 1);
    dup2(err, 2);
    close(passw);
    close(out);
    close(err);
    if ((filho = fork()) == 0)
    {
        write(0, "Input no STDIN\n", 15);
        write(1, "Input no STDOUT\n", 16);
        write(2, "Input no STDERR\n", 16);
        execl("/bin/wc", "wc", NULL);
    }
    printf("Isto vai ser escrito no saida\n");
}