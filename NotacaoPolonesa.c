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

void travesia_em_preordem(struct nodo *pai, struct nodo *filho, struct nodo **arvore, size_t count){
    if(pai != NULL && filho != NULL){
        //insere(pai, filho);
        printf("Pai: %c\n", pai->op);
        printf("Filho: %c\n", pai->op);
        travesia_em_preordem(pai->fe, arvore[count+1], arvore, count+1);
        travesia_em_preordem(pai->fd, arvore[count+2], arvore, count+2);
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
    size_t tam_op = i-1;
    while(i <= TAM_NOT){
        arvore[i] = NULL;
        i++;
    }

    // Pré-ordem para inserir
    travesia_em_preordem(arvore[0], arvore[1], arvore, 1);
    /*i = 0;
    while(i < tam_op){
        free(arvore[i]);
    }
    // Pós-ordem para dar free
    */
    return 0;
}