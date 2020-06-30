#include <stdio.h>
#include <stdlib.h>
#include "g0.h"
#define TAM 20000
#define VALOR 1

int vGlobal[TAM];
int r;
int main(int argc,char const *argv[]){
    fill(vGlobal,TAM,VALOR);
    r=find(vGlobal,TAM,1);
    printf("Encontrou com vGlobal %d\n",r);

    int* v=(int *)malloc(sizeof(int)*TAM);
    fill(v,TAM,VALOR);
    r=find(v,TAM,1);
    printf("Encontrou com malloc %d\n",r);
    r=find(v+1000,2000,1);
    printf("Encontrou entre 1000 e 2000 %d\n",r);
    free(v);


    return 0;
}