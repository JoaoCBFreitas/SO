#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>   /*chamadas ao sistema: defs e decls essenciais*/
#include <fcntl.h>    /*O_RDONLY, O_WRONLY, O_CREAT, O_* */
//-----------------------------------------------------------DONE--------------------------------
int main(int argc,char const *argv[]){
    char *buf[1024];
    int bytes=0;
    for(int i=0;i<1024;i++){
        ssize_t in=read(0,buf,sizeof(buf));
        bytes+=in; 
        write(1,buf,in);
    }
    return 0;
}