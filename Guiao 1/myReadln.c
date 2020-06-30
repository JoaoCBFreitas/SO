#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>   /*chamadas ao sistema: defs e decls essenciais*/
#include <fcntl.h>    /*O_RDONLY, O_WRONLY, O_CREAT, O_* */


//-----------------------------------DONE----------------------------------------
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
    char buf[1024];
    int bytes;
    int fich1=open(argv[1],O_RDONLY);
    if(fich1<0){
        printf("Erro a abrir o ficheiro input\n");
        close(fich1);
        return 1;
    }
    while((bytes=myreadln(fich1,buf,1))>0){
        write(1,buf,bytes);
    }
    write(1,"\n",1);
    close(fich1);
    return 0;
}