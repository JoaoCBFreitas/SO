#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>   /*chamadas ao sistema: defs e decls essenciais*/
#include <fcntl.h>    /*O_RDONLY, O_WRONLY, O_CREAT, O_* */

//-----------------------------------------DONE-----------------------------------------------------------//
int main(int argc,char const *argv[]){
    if(argc!=4) {
        printf("Nº de argumentos incorreto\n");
        return 1;
    }
    if(argc==4){
        int tam=atoi(argv[3]);
        char *buf[tam];
        int fich1=open(argv[1],O_RDONLY);
        if(fich1<0){
            printf("Erro a abrir o ficheiro input\n");
            close(fich1);
            return 1;
        }
        int fich2=open(argv[2],O_CREAT |O_TRUNC | O_WRONLY,0640);
        if(fich2<0){
            printf("Erro a abrir o ficheiro output\n");
            close(fich1);
            close(fich2);
            return 1;
        }
        int bytes=0;
        ssize_t in=1;
        while(in>0){
            in=read(fich1,buf,sizeof(buf));
            bytes+=in; 
            write(fich2,buf,in);
        }
        close(fich1);
        close(fich2);
    }
    
    return 0;
}