#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    execlp("/mnt/c/Users/joaob/Desktop/Universidade/SO/Guiao 3/ex3", "-lasda", "-la", "ra", NULL);
    return 0;
}