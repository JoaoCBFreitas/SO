#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h> /*chamadas ao sistema: defs e decls essenciais*/
#include <fcntl.h>  /*O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include "pessoas.h"

int new_person(char *name, int age)
{
    int fd = open("pessoas.txt", O_CREAT | O_APPEND | O_RDWR, 0644);
    int writtenB = 0;
    if (fd < 0)
    {
        perror("Erro a abrir o ficheiro\n");
        close(fd);
        return -1;
    }
    Person p;
    strncpy(p.name, name, 256);
    p.age = age;
    writtenB += lseek(fd, 0, SEEK_END);
    writtenB += write(fd, &p, sizeof(struct Person));
    close(fd);
    return writtenB;
}
int person_change_age(char *name, int age)
{
    int fd = open("pessoas.txt", O_RDWR, 0644);
    ssize_t readByte = 0;
    int byte = 1;
    Person p;
    while (byte > 0)
    {
        byte = read(fd, &p, sizeof(struct Person));
        readByte += byte;
        char *buf = p.name;
        if (byte == 0)
        {
            write(1, "Pessoa nao existe\n", 19);
            break;
        }
        if (strcmp(buf, name) == 0)
        {
            Person p2;
            strncpy(p2.name, name, 256);
            p2.age = age;
            lseek(fd, -byte, SEEK_CUR);
            printf("Encontrou %s-%d\n", p.name, p.age);
            printf("Mudou para %s-%d\n", p2.name, p2.age);
            write(fd, &p2, sizeof(struct Person));
            break;
        }
    }
    close(fd);
    return 0;
}
int person_change_age_v2(long pos, int age)
{
    int fd = open("pessoas.txt", O_RDWR, 0644);
    lseek(fd, pos, SEEK_SET);
    Person p;
    int byte = read(fd, &p, sizeof(struct Person));
    if (byte > 0)
    {
        lseek(fd, -byte, SEEK_CUR);
        Person p2;
        strncpy(p2.name, p.name, 256);
        p2.age = age;
        printf("Encontrou %s-%d\n", p.name, p.age);
        printf("Mudou para %s-%d\n", p2.name, p2.age);
        write(fd, &p2, sizeof(struct Person));
    }
    else
    {
        write(1, "Pessoa nao existe\n", 19);
    }
    close(fd);
    return 0;
}
int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        write(1, "Inputs insuficientes\n", 22);
        return -1;
    }
    int r = 0;
    if (strcmp(argv[1], "-i") && strcmp(argv[1], "-u"))
    {
        write(1, "Inputs inválidos\n", 19);
    }
    else
    {
        if (strcmp(argv[1], "-i") == 0)
        {
            r += new_person(argv[2], atoi(argv[3]));
            printf("%ld\n", r - sizeof(struct Person));
        }
        if (strcmp(argv[1], "-u") == 0)
        {
            //person_change_age(argv[2],atoi(argv[3]));
            person_change_age_v2(atoi(argv[2]), atoi(argv[3]));
        }
    }
    return 0;
}