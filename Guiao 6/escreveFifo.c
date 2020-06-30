#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int fd;
    char *fifo = "/tmp/myfifo";
    char arr2[80];
    while (1)
    {
        // Open FIFO for write only
        if ((fd = open(fifo, O_WRONLY)) == -1)
        {
            printf("Something went wrong");
            return EXIT_FAILURE;
        }

        // Take an input arr2ing from user.
        // 80 is maximum length
        fgets(arr2, 80, stdin);

        // Write the input arr2ing on FIFO
        // and close it
        write(fd, arr2, strlen(arr2) + 1);
        close(fd);
    }
    return 0;
}

//int mkfifo(const char *path, mode_t mode)
//cria um named pipe com o nome path e permissoes acesso (0->sucesso,-1->erro)
//(unlink(path))