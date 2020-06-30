#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include "argus.h"
int inatividade = 5;
int exec = 10;
int tarefa = 1;
/**
 * Função que cria um named Pipe, eliminando o antigo se necessário
 **/
int createNewFifo(const char *fifoName)
{
    struct stat stats;
    if (stat(fifoName, &stats) < 0)
    {
        if (errno != ENOENT) // ENOENT is ok, since we intend to delete the file anyways
        {
            perror("stat failed"); // any other error is a problem
            return (-1);
        }
    }
    else // stat succeeded, so the file exists
    {
        if (unlink(fifoName) < 0) // attempt to delete the file
        {
            perror("unlink failed"); // the most likely error is EBUSY, indicating that some other process is using the file
            return (-1);
        }
    }

    if (mkfifo(fifoName, 0666) < 0) // attempt to create a brand new FIFO
    {
        perror("mkfifo failed");
        return (-1);
    }

    return (0);
}
/**
 * Função que lista os comandos em execução 
 */
char *listagem()
{
    int fd = open("tarefas.bin", O_RDONLY, 0644);
    lseek(fd, 0, SEEK_SET);
    ssize_t readByte = 0;
    int byte = 1;
    char *buffer = malloc(sizeof(char) * 10000);
    Tarefa t;
    while (byte > 0)
    {
        byte = read(fd, &t, sizeof(struct Tarefa));
        readByte += byte;
        if (byte == 0)
        {
            break;
        }
        else
        {
            if (strcmp(t.estado, "executar") == 0)
            {
                char str[100];
                sprintf(str, "%d", t.tarefa);
                strcat(buffer, "#");
                strcat(buffer, str);
                strcat(buffer, ": ");
                strcat(buffer, t.comando);
                strcat(buffer, "\n");
            }
        }
    }
    close(fd);
    return buffer;
}
/**
 * Função que lista os comandos já executados
 * */
char *leHistorico()
{
    int fd = open("tarefas.bin", O_RDONLY, 0644);
    lseek(fd, 0, SEEK_SET);
    ssize_t readByte = 0;
    int byte = 1;
    char *buffer = malloc(sizeof(char) * 10000);
    Tarefa t;
    while (byte > 0)
    {
        byte = read(fd, &t, sizeof(struct Tarefa));
        readByte += byte;
        if (byte == 0)
        {
            break;
        }
        else
        {
            if (strcmp(t.estado, "concluida") == 0 || strcmp(t.estado, "max inatividade") == 0 || strcmp(t.estado, "terminado") == 0 || strcmp(t.estado, "max execução") == 0)
            {
                char str[100];
                sprintf(str, "%d", t.tarefa);
                strcat(buffer, "#");
                strcat(buffer, str);
                strcat(buffer, ", ");
                strcat(buffer, t.estado);
                strcat(buffer, ": ");
                strcat(buffer, t.comando);
                strcat(buffer, "\n");
            }
        }
    }
    close(fd);
    return buffer;
}
/**
 * Altera uma certa tarefa no ficheiro
 **/
int alteraLog(Tarefa t, int i)
{
    int fd = open("tarefas.bin", O_RDWR, 0644);
    ssize_t readByte = 0;
    int byte = 1;
    Tarefa t2;
    while (byte > 0)
    {
        byte = read(fd, &t2, sizeof(struct Tarefa));
        readByte += byte;
        int num = t2.tarefa;
        if (byte == 0)
        {
            break;
        }
        if (num == i)
        {
            lseek(fd, -byte, SEEK_CUR);
            write(fd, &t, sizeof(struct Tarefa));
            break;
        }
    }
    close(fd);
    return 0;
}
/**
 * Adiciona uma tarefa ao ficheiro log
 **/
void adicionaLog(Tarefa t)
{
    int fd = open("tarefas.bin", O_APPEND | O_RDWR, 0644);
    if (fd < 0)
    {
        perror("Erro a abrir o ficheiro\n");
        close(fd);
        exit(1);
    }
    lseek(fd, 0, SEEK_END);
    write(fd, &t, sizeof(struct Tarefa));
    close(fd);
}
/**
 * Função que executa um certo comando
 * */
void execCom(char *args)
{
    char *aux = strtok(args, " ");
    char *argv[10];
    int i = 0;
    while (aux != NULL)
    {
        argv[i] = aux;
        aux = strtok(NULL, " ");
        i++;
    }
    argv[i] = NULL;
    execvp(argv[0], argv);
}
/**
 * Função que altera o timeout de inatividade
 */
void tempoInatividade(char *args)
{
    inatividade = atoi(args);
}
/**
 * Função que altera o timeout de execução
 */
void tempoExec(char *args)
{
    exec = atoi(args);
}
/**
 * Handler timeout de execução
 */
void timeout(int signum)
{
    int f = 0;
    int fd = open("tarefas.bin", O_RDWR, 0644);
    ssize_t readByte = 0;
    int byte = 1;
    int *num;
    Tarefa t;
    while (byte > 0)
    {
        byte = read(fd, &t, sizeof(struct Tarefa));
        readByte += byte;
        num = t.pid;
        if (byte == 0)
        {
            break;
        }
        for (int i = 0; num[i] != -1; i++)
        {
            if (num[i] == getpid())
            {
                f = 1;
            }
        }
        if (f == 1)
            break;
    }
    close(fd);
    if (f == 1)
    {
        t.estado = "max execução";
        alteraLog(t, t.tarefa);
        int i = 0;
        for (; num[i] != -1; i++)
            ;
        for (i--; i >= 0; i--)
        {
            kill(num[i], SIGTERM);
        }
    }
}
/**
 * Handler timeout de inatividade
 */
void timeout2(int signum)
{
    int f = 0;
    int fd = open("tarefas.bin", O_RDWR, 0644);
    ssize_t readByte = 0;
    int byte = 1;
    int *num;
    Tarefa t;
    while (byte > 0)
    {
        byte = read(fd, &t, sizeof(struct Tarefa));
        readByte += byte;
        num = t.pid;
        if (byte == 0)
        {
            break;
        }
        for (int i = 0; num[i] != -1; i++)
        {
            if (num[i] == getpid())
            {
                f = 1;
            }
        }
        if (f == 1)
            break;
    }
    close(fd);
    if (f == 1)
    {
        t.estado = "max inatividade";
        alteraLog(t, t.tarefa);
        int i = 0;
        for (; num[i] != -1; i++)
            ;
        for (i--; i >= 0; i--)
        {
            if (num[i] == getpid())
                continue;
            kill(num[i], SIGTERM);
        }
    }
    kill(getpid(), SIGTERM);
}
/**
 * Handler de sigINT
 * */
void killhandler(int signum)
{
    unlink("/tmp/myfifo");
    kill(0, SIGKILL);
}
/**
 * Função que gera os pipes anonimos para correr vários comandos em pipes
 * 
 **/
void piping(char *argv[], int argc, Tarefa t)
{
    adicionaLog(t);
    int fd[2];
    pid_t pid;
    int fdd = 0; /* Backup */
    int i = 0;
    while (i < argc)
    {
        if (pipe(fd) == -1)
        {
            perror("Pipe failed");
            exit(1);
        }
        if ((pid = fork()) == -1)
        {
            perror("fork failed");
            exit(1);
        }
        if (pid == 0)
        {
            if (i < argc - 1)
            {
                signal(SIGALRM, timeout2);
                pid_t pid2;
                if ((pid2 = fork()) == -1)
                {
                    perror("fork failed");
                    exit(1);
                }
                if (pid2 == 0)
                {
                    int j;
                    for (j = 0; t.pid[j] != -1; j++)
                        ;
                    t.pid[j] = getpid();
                    t.pid[j + 1] = getppid();
                    alteraLog(t, t.tarefa);
                    dup2(fdd, 0);
                    if (argv[i + 1] != NULL)
                    {
                        dup2(fd[1], 1);
                        dup2(fd[1], 2);
                    }
                    close(fd[0]);
                    execCom(argv[i]);
                    perror("Comand Failed");
                    exit(EXIT_FAILURE);
                    i++;
                }
                else
                {
                    alarm(inatividade);
                    waitpid(pid2, 0, WUNTRACED);
                    exit(0);
                }
            }
            else
            {
                int j;
                for (j = 0; t.pid[j] != -1; j++)
                    ;
                t.pid[j] = getpid();
                alteraLog(t, t.tarefa);
                dup2(fdd, 0);
                if (argv[i + 1] != NULL)
                {
                    dup2(fd[1], 1);
                    dup2(fd[1], 2);
                }
                close(fd[0]);
                execCom(argv[i]);
                perror("Comand Failed");
                exit(EXIT_FAILURE);
                i++;
            }
        }
        else
        {
            wait(NULL); /* Collect childs */
            close(fd[1]);
            fdd = fd[0];
            i++;
        }
    }
}
/**
 * Encontra uma tarefa i 
 **/
Tarefa terminate(int i)
{
    int fd = open("tarefas.bin", O_RDWR, 0644);
    ssize_t readByte = 0;
    int byte = 1;
    Tarefa t;
    while (byte > 0)
    {
        byte = read(fd, &t, sizeof(struct Tarefa));
        readByte += byte;
        int num = t.tarefa;
        if (byte == 0)
        {
            break;
        }
        if (num == i)
        {
            break;
        }
    }
    close(fd);
    return t;
}
/**
 * Função que separa os comandos de acordo com | para depois serem executados
 * */
void executar(char *args, Tarefa t)
{
    char *cmdstrdup = strdup(args);
    char *saveptr;
    char *ptr;
    char *argv[20];
    int argc;

    if ((ptr = strtok_r(cmdstrdup, "|", &saveptr)) == NULL)
    {
        printf("%s\n", "no args given");
        return;
    }

    argv[argc = 0] = cmdstrdup;
    while (ptr != NULL)
    {
        ptr = strtok_r(NULL, "|", &saveptr);
        if (++argc >= 20 - 1) // -1 for room for NULL at the end
            break;
        argv[argc] = ptr;
    }
    alarm(exec);
    piping(argv, argc, t);
    t.estado = "concluida";
    alteraLog(t, t.tarefa);
    free(cmdstrdup);
}
/**
 * Lista os comandos em execução
 * */
char *listar()
{
    char *list = listagem();
    return list;
}
/**
 * Termina uma tarefa e muda o seu estado no ficheiro
 * 
 */
char *terminar(Tarefa t)
{
    char *res = malloc(sizeof(char) * 50);
    if (t.comando != NULL)
    {
        int i;
        for (int j = 1; t.pid[j] != -1; j++)
        {
            i += kill(t.pid[j], SIGTERM);
        }
        char str[100];
        sprintf(str, "%d", t.tarefa);
        strcpy(res, "Tarefa #");
        strcat(res, str);
        strcat(res, " Terminada\n");
    }
    else
    {
        strcpy(res, "Tarefa inválida\n");
    }
    return res;
}
/**
 * Lista o historico de comandos com o seu estado final
 */
char *historico()
{
    char *hist = leHistorico();
    return hist;
}
/**
 * Lê os argumentos do array input
 * */
char *leArgumentos(char *arr2)
{
    char *args = malloc(sizeof(char) * 200);
    int j = 0;
    for (int i = 3; arr2[i] != '\0'; i++, j++)
    {
        args[j] = arr2[i];
    }
    args[j] = '\0';
    return args;
}
/**
 * Lê os comandos do array input
 */
char *leComando(char *arr2)
{
    char *comando = malloc(sizeof(char) * 2);
    comando[0] = '-';
    comando[1] = arr2[1];
    return comando;
}
int main(int argc, char *argv[])
{
    int fd;
    char arr2[120];
    char *resposta = malloc(sizeof(char) * 1024);
    char *fifo = "/tmp/myfifo";
    int file = open("tarefas.bin", O_CREAT | O_TRUNC, 0644); //criação do ficheiro das tarefas
    close(file);
    if (createNewFifo(fifo) < 0)
        exit(1);
    signal(SIGALRM, timeout);
    signal(SIGCHLD, SIG_IGN); //elimina os zombies
    signal(SIGINT, killhandler);
    signal(SIGQUIT, killhandler);
    while (1)
    {
        //----------------------------Receber comando do cliente----------------------------//
        // Open FIFO for Read only
        if ((fd = open(fifo, O_RDONLY)) == -1)
        {
            write(1, "Something went wrong\n", 22);
            return EXIT_FAILURE;
        }
        // Read from FIFO
        read(fd, arr2, sizeof(arr2));
        char *comando = leComando(arr2);

        if (strcmp(comando, "-i") == 0)
        {
            char *args = leArgumentos(arr2);
            tempoInatividade(args);
            strcpy(resposta, "Inatividade=");
            strcat(resposta, args);
            strcat(resposta, "\n");
            free(args);
        }
        else if (strcmp(comando, "-m") == 0)
        {
            char *args = leArgumentos(arr2);
            tempoExec(args);
            strcpy(resposta, "Execução=");
            strcat(resposta, args);
            strcat(resposta, "\n");
            free(args);
        }
        else if (strcmp(comando, "-e") == 0)
        {
            char *args = leArgumentos(arr2);
            pid_t pid = fork();
            Tarefa t;
            t.tarefa = tarefa;
            t.comando = args;
            t.estado = "executar";
            if (pid == 0)
            {
                t.pid[0] = getpid();
                for (int i = 1; i < 20; i++)
                {
                    t.pid[i] = -1;
                }
                executar(args, t);
                wait(NULL);
                free(args);
                exit(0);
            }
            else
            {
                char str[100];
                sprintf(str, "%d", t.tarefa);
                strcpy(resposta, "nova tarefa #");
                strcat(resposta, str);
                strcat(resposta, "\n");
                tarefa++;
            }
        }
        else if (strcmp(comando, "-l") == 0)
        {
            resposta = listar();
        }
        else if (strcmp(comando, "-t") == 0)
        {
            char *args = leArgumentos(arr2);
            Tarefa t2 = terminate(atoi(args));
            resposta = terminar(t2);
            kill(t2.pid[0], SIGTERM);
            t2.estado = "terminado";
            alteraLog(t2, t2.tarefa);
            free(args);
        }
        else if (strcmp(comando, "-r") == 0)
        {
            resposta = historico();
        }
        else
        {
            continue;
        }
        free(comando);
        close(fd);
        //----------------------------Enviar reposta para o cliente----------------------------//
        // Open FIFO for write only
        if ((fd = open(fifo, O_WRONLY)) == -1)
        {
            write(1, "Something went wrong\n", 22);
            return EXIT_FAILURE;
        }
        write(fd, resposta, strlen(resposta) + 1);
        close(fd);
    }
    return 0;
}
