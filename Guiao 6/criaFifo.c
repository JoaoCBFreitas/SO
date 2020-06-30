#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    char *fifo = "/tmp/myfifo";
    if (mkfifo(fifo, 0666) == -1)
    {
        printf("Something went wrong\n");
        return EXIT_FAILURE;
    }
    return 0;
}

//int mkfifo(const char *path, mode_t mode)
//cria um named pipe com o nome path e permissoes acesso (0->sucesso,-1->erro)
//(unlink(path))