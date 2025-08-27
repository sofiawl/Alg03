//-----------------------------------------------------------------
//   ÁRVORES COM NODOS IRMÃOS E ÁRVORES REPRESENTADAS EM MATRIZ
//      Desenvolvido por Sofia Wamser Lima 20/Ago/2025
//-----------------------------------------------------------------
// Disciplina de Algorítmos III
#include <stdio.h>
#include <stdlib.h>

struct nodo{
    int chave;
    struct nodo* irmaod;
    struct nodo* fe;
};

void inserir_nodos(struct nodo* pai, int *chave, int *quant_fil){

    printf("Digite a quantidade de filhos de %d: \n", pai->chave);
    if (scanf("%d", quant_fil) != 1){
        printf("Valor inválido\n");
        return;
    }

    for (int i = 0; i < *quant_fil; i++){

        struct nodo *filho = malloc(sizeof(struct nodo));
        

        printf("Digite o valor do filho %d de %d: \n",i+1, pai->chave);
        if (scanf("%d", chave) != 1){
            printf("Valor inválido\n");
            return;
        }

        filho->chave = *chave;
        filho->fe = NULL;
        filho->irmaod = NULL;

        if (i == 0){
            pai->fe = filho;
        } else {
            pai->fe->irmaod = filho;
        }

        inserir_nodos(filho, chave, quant_fil);
    }

    return;
}

int main(){
    int chave, quant_fil;

    printf("Digite um valor inteiro para a raiz: \n");
    if (scanf("%d", &chave) != 1){
        printf("Valor inválido\n");
        return 1;
    }
    struct nodo* raiz = malloc(sizeof(struct nodo));
    raiz->chave = chave;

    inserir_nodos(raiz, &chave, &quant_fil);

    return 0;
}