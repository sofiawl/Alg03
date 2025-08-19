#include <stdio.h>

struct nodo{
    int chave;
    struct nodo *fe;
    struct nodo *fd;
}

struct nodo* DFS(struct nodo *no, int x){
    if (no == NULL)
        return NULL;

    if (no->chave == x)
        return no;

    struct nodo *retorno = DFS(no->fe, x);
    if (retorno)
        return retorno;

    retorno = DFS(no->fd, x): 

    return retorno;
}

struct nodo* DFS_SR(struct nodo *no, int x, struct nodo **pilha){
    if (no == NULL)
        return NULL;

    int i = 0;
    pilha(i++) = no;

    while(i > 0){
        struct nodo* aux = pilha(i--);
        if (aux->chave == x)
            return aux;
        if (aux->fd)
            pilha(i++) = aux->fd;
        if (aux->fe)
            pilha(i++) = aux->fe; 
    }
    
    return NULL;
}

void BFS_SR(struct nodo *no, int x, struct nodo **fila){
    int i = 0;
    fila(i++) = no;

    while(i > 0){
        struct nodo *aux = fila(1);
        i--;
        if (no->chave == x)
            return no;
        if (no->fe)
            fila(i++) = no->fe;
        if (no->fd)
            fila(i++) = no->fd;
    }
    
}

int main(){

}