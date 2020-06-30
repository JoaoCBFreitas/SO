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
    char arr1[80];
    while (1)
    {
        // Open FIFO for Read only
        if ((fd = open(fifo, O_RDONLY)) == -1)
        {
            printf("Something went wrong");
            return EXIT_FAILURE;
        }
        // Read from FIFO
        read(fd, arr1, sizeof(arr1));

        // Print the read message
        printf("%s\n", arr1);
        close(fd);
    }
    return 0;
}

//int mkfifo(const char *path, mode_t mode)
//cria um named pipe com o nome path e permissoes acesso (0->sucesso,-1->erro)
//(unlink(path))