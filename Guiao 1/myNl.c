#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>   /*chamadas ao sistema: defs e decls essenciais*/
#include <fcntl.h>    /*O_RDONLY, O_WRONLY, O_CREAT, O_* */

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

ssize_t myreadln(int fildes, char *buf, size_t size){
    int next_pos=0;
    int read_bytes=0;
	while(next_pos<size && read(fildes,buf+next_pos,1)>0){
        read_bytes++;
        if(buf[next_pos]=='\n'){
            read_bytes=-2;
            break;
        }
        next_pos++;
    }
    return read_bytes;
}
int main(int argc,char const *argv[]){
    char buf1[1024];
    char buf2[1024];
    int bytes=1;
    int fich1=open(argv[1],O_RDONLY);
    if(fich1<0){
        printf("Erro a abrir o ficheiro input\n");
        close(fich1);
        return 1;
    }
    int i=1;
    if(atoi(argv[2])==1){
        while(1){
            bytes=1;
            char linha[12];
            sprintf(linha,"   %d   ",i);
            write(1,linha,9);
            while((bytes=myreadln(fich1,buf1,1))>0) write(1,buf1,bytes);
            if(bytes==0) break;
            i++;
            write(1,"\n",1);
        }
    }
    if(atoi(argv[2])==2){
        //myreadln2 nao esta 100% correto (ciclo nao acaba)
        i=1;
        while(1){
            char linha[12];
            sprintf(linha,"   %d   ",i);
            write(1,linha,9);
            int nbytes=0;
            if((bytes=myreadln2(fich1,buf2,sizeof(buf2)))>0){
                nbytes+=bytes;
                write(1,buf2,nbytes);
                write(1,"\n",1);
            }else{
                break;
            }
            i++;
        }
    }
    write(1,"\n",1);
    close(fich1);
    return 0;
}