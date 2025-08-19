//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// CALCULADORA DE NOTAÇÃO POLONESA
// Desenvolvida por Sofia Wamser Lima em 16/Ago/2025

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <stdlib.h>
#include <stdio.h>

#define TAM_NOT 20
struct nodo{
    char op;
    float val;
    struct nodo* fe;
    struct nodo* fd;
};

void insere(struct nodo* pai, struct nodo* filho){
    if (!pai || !filho)
        return;
    if (!pai->fe)
        pai->fe = filho;
    else if (!pai->fd)
        pai->fd = filho;
    else
        printf("Erro ao adicionar novo nodo\n");
}

void travesia_em_preordem(struct nodo *pai, struct nodo *filho, struct nodo **arvore, size_t count, size_t tam){
    if (count >= tam) return;

    if(pai != NULL && filho != NULL){
        insere(pai, filho);
        printf("Pai: %c\n", pai->op);
        printf("Filho: %c\n", filho->op);
        travesia_em_preordem(pai->fe, arvore[count+1], arvore, count+1, tam);
        travesia_em_preordem(pai->fd, arvore[count+2], arvore, count+2, tam);
    }
}

void posordem(){
    
}

int main(){
    char nota[TAM_NOT];

    fgets(nota, sizeof(nota), stdin);
    size_t tam_arvore = sizeof(nota) / sizeof(char);
    printf("Operação: %s\n", nota);
    struct nodo *arvore[tam_arvore];

    size_t i = 0;
    while(nota[i] != '\0'){
        struct nodo* no = malloc(sizeof(struct nodo));
        no->op = nota[i];
        printf("init%ld: %c ",i, no->op);
        arvore[i] = no;
        i++;
    }

    // Pré-ordem para inserir
    travesia_em_preordem(arvore[0], arvore[1], arvore, 1, i-1);

    /*i = 0;
    while(i < tam_op){
        free(arvore[i]);
    }
    // Em ordem para ler
    
    // Pós-ordem para dar free
    */
    return 0;
}