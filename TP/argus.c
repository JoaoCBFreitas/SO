#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include "argus.h"

void printMenu()
{
    write(1, "-------MENU-------\n", 20);
    write(1, "1#- Mudar tempo de inactividade\n", 33);
    write(1, "2#- Mudar tempo de execução\n", 31);
    write(1, "3#- Executar uma tarefa\n", 25);
    write(1, "4#- Listar tarefas em execução\n", 34);
    write(1, "5#- Terminar tarefas em execução\n", 36);
    write(1, "6#- Histórico\n", 16);
    write(1, "7#- Ajuda\n", 11);
    write(1, "0#- Sair\n", 10);
    write(1, "------------------\n", 20);
    write(1, "Escolha a sua opção\n", 23);
}
int main(int argc, char *argv[])
{
    int fd;
    char *fifo = "/tmp/myfifo";
    char *option = malloc(sizeof(char) * 2);
    char arg[100];
    if (argc < 2)
    {
        char escolha[2];
        int aux;
        char time[4];
        int tempo = 0;
        while (1)
        {
            printMenu();
            read(0, &escolha, 1);
            escolha[1] = '\0';
            aux = atoi(escolha);
            if (aux >= 0 && aux < 8)
                break;
        }
        int c;
        while ((c = fgetc(stdin)) != EOF && c != '\n') //Limpa o newline do STDIN
            ;
        switch (aux)
        {
        case 0:
            write(1, "A sair\n", 8);
            exit(0);
        case 1:
            strcpy(option, "-i");
            while (tempo <= 0 || tempo > 999)
            {
                write(1, "Indique o tempo máximo de inatividade\n", 40);
                read(0, &time, 3);
                time[3] = '\0';
                tempo = atoi(time);
            }
            strcpy(arg, time);
            break;
        case 2:
            strcpy(option, "-m");
            while (tempo <= 0 || tempo > 999)
            {
                write(1, "Indique o tempo máximo para execução\n", 41);
                read(0, &time, 3);
                time[3] = '\0';
                tempo = atoi(time);
            }
            strcpy(arg, time);
            break;
        case 3:
            strcpy(option, "-e");
            write(1, "Indique os comandos a executar\n", 32);
            int nbytes = read(0, &arg, sizeof(arg));
            arg[nbytes - 1] = '\0';
            break;
        case 4:
            strcpy(option, "-l");
            strcpy(arg, "ignore");
            break;
        case 5:
            strcpy(option, "-t");
            while (tempo <= 0 || tempo > 999)
            {
                write(1, "Indique a tarefa a terminar\n", 29);
                read(0, &time, 3);
                time[3] = '\0';
                tempo = atoi(time);
            }
            strcpy(arg, time);
            break;
        case 6:
            strcpy(option, "-r");
            strcpy(arg, "ignore");
            break;
        case 7:
            strcpy(option, "-h");
            strcpy(arg, "ignore");
            break;
        }
    }
    else
    {
        option = argv[1];
        if (argc > 2)
        {
            strcpy(arg, argv[2]);
        }
    }
    if (strcmp(option, "-h") == 0)
    {
        write(1, "tempo-inatividade segs--> '-i n'\n", 34);
        write(1, "tempo-execucao segs--> '-m n'\n", 31);
        write(1, "executar comando--> '-e \"comando\"'\n", 36);
        write(1, "comandos em execução--> '-l'\n", 32);
        write(1, "terminar tarefa--> '-t n'\n", 27);
        write(1, "historico--> '-r'\n", 19);
        write(1, "ajuda--> '-h'\n", 15);
        exit(0);
    }
    char arr1[120] = "";
    char *res = malloc(sizeof(char) * 200);
    while (1)
    {
        //----------------------------Enviar comando para o servidor----------------------------//
        // Open FIFO for write only
        if ((fd = open(fifo, O_WRONLY)) == -1)
        {
            write(1, "Something went wrong\n", 22);
            return EXIT_FAILURE;
        }
        //Comandos através da shell
        if (argc == 2)
        {
            strcat(arr1, option);
        }
        //comandos através do menu
        else if ((strcmp(arg, "ignore") != 0))
        {
            strcpy(arr1, option);
            strcat(arr1, " ");
            strcat(arr1, arg);
        }
        else
        {
            strcat(arr1, option);
        }
        write(fd, arr1, strlen(arr1) + 1);
        close(fd);
        //----------------------------Enviar reposta do servidor----------------------------//
        // Open FIFO for Read only
        if ((fd = open(fifo, O_RDONLY)) == -1)
        {
            write(1, "Something went wrong\n", 22);
            return EXIT_FAILURE;
        }
        read(fd, res, 1024);
        write(1, res, strlen(res) + 1);
        break;
    }
    return 0;
}
