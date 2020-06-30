#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>   /*chamadas ao sistema: defs e decls essenciais*/
#include <fcntl.h>    /*O_RDONLY, O_WRONLY, O_CREAT, O_* */
//-------------------------------DONE--------------------------
ssize_t readch(int fildes, char* buf){
    char* c=buf;
    read(fildes,c,1);
    if(*c=='\n') {
        return -2;
    }else{
        buf=c;
        return 1;
    } 
}

ssize_t myreadln2(int fildes, char *buf, size_t size){
    int next_pos=0;
    int read_bytes=0;
    ssize_t s=1;
	while(next_pos<size && (s=readch(fildes,buf+next_pos))>0){
        read_bytes++;
        next_pos++;
    }
    return read_bytes;
}
int main(int argc,char const *argv[]){
    char buf[1024];
    int bytes;
    int fich1=open(argv[1],O_RDONLY);
    if(fich1<0){
        printf("Erro a abrir o ficheiro input\n");
        close(fich1);
        return 1;
    }
    int nbytes=0;
    if((bytes=myreadln2(fich1,buf,sizeof(buf)))>0){
        nbytes+=bytes;
    }
    write(1,buf,nbytes);
    write(1,"\n",1);
    close(fich1);
    return 0;
}