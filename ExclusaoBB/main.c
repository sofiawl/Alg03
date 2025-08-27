#include <stdio.h>
#include <stdlib.h>

struct nodoBB{
    int key;
    struct nodoBB *ls;
    struct nodoBB *rs;
    struct nodoBB *dad;
};

// Substitui uma subárvore por outra, a substituída fica sem nada
void transplantar(struct nodoBB *src, struct nodoBB *old, struct nodoBB *new){
    if (old->dad == NULL)
        src = new;
    else if(old->key == old->dad->ls)
        src->ls = new;
    else 
        src->rs = new;

    if (new != NULL)
        new->dad = old->dad;
    
    return;
}

struct nodoBB* min_nodo(struct nodoBB *src){
    if (src)
        return -1;

    struct nodoBB *aux = src;
    while(aux){
        aux = aux->ls; 
    }

    return aux;
}

// Excluir o nodo rm da árvore
void excluir(struct nodoBB *src, struct nodoBB *rm){
    if (rm->ls == NULL){
        transplantar(src, rm, rm->ls);
        return;
    }
    if (rm->rs == NULL){
        transplantar(src, rm, rm->rs);
        return;
    }

    struct nodoBB *min = min_nodo(src);
    
    if (min != rm->rs){
        transplantar(src, rm, rm->rs);
        min->rs = rm->rs;
        min->rs->dad = min;
    }
    transplantar(src, rm, min);
    min->ls = rm->ls;
    min->ls->dad = min;

    return;
}

int main(){
    
}