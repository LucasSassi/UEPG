#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int exercicioTeste(int n){
    if (n ==  1){
        return 1;
    }

    return (n * n * n) + exercicioTeste(n-1);
}

int main(){

int n;
n = exercicioTeste(4); 
printf("\n%d\n", n);
}