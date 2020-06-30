#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    int passw = open("passwd.txt", O_CREAT | O_TRUNC | O_WRONLY, 0640);
    int out = open("saida.txt", O_CREAT | O_TRUNC | O_WRONLY, 0640);
    int err = open("erros.txt", O_CREAT | O_TRUNC | O_WRONLY, 0640);
    dup2(passw, 0);
    dup2(out, 1);
    dup2(err, 2);
    //fechar visto que já não é preciso
    close(passw);
    close(out);
    close(err);
    write(0, "Input no STDIN\n", 15);
    write(1, "Input no STDOUT\n", 16);
    write(2, "Input no STDERR\n", 16);
    printf("Isto vai ser escrito no passwd\n");
    close(0);
    close(1);
    close(2);
}