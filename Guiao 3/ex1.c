#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    execl("/bin/ls", "-l", NULL);
    return 0;
}