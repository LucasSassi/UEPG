#include <stdlib.h>
#include <stdio.h>

int main(){
    // FILE *fp;
    // fp = fopen("exemplo1.bin", "wb");

    // if (fp == NULL) { printf("Erro ao abrir o arquivo.\n"); }

    //     fclose(fp);

    //     return 0;


    FILE *arq;
    char string[100];
    int i;
    arq = fopen("exemplo2.txt", "w");
    if (arq == NULL) { printf("Erro ao abrir o arquivo.\n");
        system("pause");
        exit(1);
    }
    printf("Digite uma string: ");
    fgets(string, sizeof(string), stdin);
    fprintf(arq, "%s", string);
    fclose(arq);

    // Leitura do arquivo
    arq = fopen("exemplo2.txt", "r");
    if (arq == NULL) { printf("Erro ao abrir o arquivo.\n");
    
        system("pause");
        exit(1);
    }
    printf("Conteúdo do arquivo:\n");

    char c;

    while ((c = fgetc(arq)) != EOF) {
        printf("%s", string);
        fclose(arq);
        system("pause");
        exit(1);
    }

    // for (i = 0; i < 100; i++) {
    //     char c = fgetc(arq);
    //     if (c == EOF) { break; }
    //     putchar(c);
    // }

    fclose(arq);
    system("pause");

    }

    // Teste de commit
    