#include <stdio.h>
#include <stdlib.h>
#include "g0.h"

void fill(int* vector, int size, int value){
    for(int i=0;i<size;i++){
        vector[i]=value;
    }
}
int find(int* vector,int size,int value){
    int r=0;
    for(int i=0;i<size;i++){
        if(vector[i]==value) r++;
    }
    return r;
}