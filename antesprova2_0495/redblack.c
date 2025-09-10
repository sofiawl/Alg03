#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include "redblack.h"

void matarProgramaFaltaMemoria(){};

struct aluno* getAluno(){
    struct aluno* retorno = malloc(sizeof(struct aluno));
    if(!retorno)
        matarProgramaFaltaMemoria();
    retorno->nome = malloc(sizeof("SOFIA WAMSER LIMA"));//sizeof conta o \0
	if(!retorno->nome)
		matarProgramaFaltaMemoria();
    retorno->nomeDinf = malloc(sizeof("swl24"));
    if(!retorno->nomeDinf)
        matarProgramaFaltaMemoria();
    
	strcpy(retorno->nome, "SOFIA WAMSER LIMA");
    strcpy(retorno->nomeDinf, "swl24");
    retorno->grr = 495;

	return retorno;
}

void imprimirDadosAluno(){
    struct aluno* aluno = getAluno();
    
    printf("Trabalho de %s\n", aluno->nome);
    printf("Login Dinf %s\n", aluno->nomeDinf);
    printf("GRR %0u\n\n", aluno->grr);

    free(aluno->nome);
    free(aluno->nomeDinf);
    free(aluno);

    return;
}

void rotacionarEsq(struct nodo** raiz, struct nodo *x){
    if (!raiz || !x) return;

    // devo garantir que x nem y são sentinelas
    struct nodo* y = x->fd;
    x->fd = y->fe;

    if (y->fe->chave != SENTINELA) // se fe não é sentinela
        y->fe->pai = x;

    y->pai = x->pai;

    if (x->pai->chave == SENTINELA) // se x for a raiz
        *raiz = y;
    else if (x == x->pai->fe)
        x->pai->fe = y;
    else 
        x->pai->fd = y;
    

    y->fe = x;
    x->pai = y;
}

void rotacionarDir(struct nodo** raiz, struct nodo* x){
    if (!raiz || !x) return;

    // devo garantir que x nem y são sentinelas
    struct nodo* y = x->fe;
    x->fe = y->fd;

    if (y->fd->chave != SENTINELA) // se fd não é sentinela 
        y->fd->pai = x;

    y->pai = x->pai;

    if (x->pai->chave == SENTINELA) // se x for a raiz
        *raiz = y;
    else if (x == x->pai->fe)
        x->pai->fe = y;
    else 
        x->pai->fd = y;
    

    y->fd = x;
    x->pai = y;
}

void corrigirInserir(struct nodo** raiz, struct nodo* z){
    while(z->pai->cor == VERMELHO){
        if (z->pai == z->pai->pai->fe) // pai de z é filho esquerdo
        {
            struct nodo* tio_z = z->pai->pai->fd; // y é tio de z
            if (tio_z->cor == PRETO) // pai e tio de z não vermelhor
            {
                z->pai->cor  = PRETO;
                tio_z->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            }
            else
            {
                // caso 1
                if (z == z->pai->fd)
                {
                    z = z->pai;
                    rotacionarEsq(raiz, z);
                }
                // caso 2
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacionarDir(raiz, z->pai->pai);
            }
        }  
        else // pai de z é filho direito
        {
            struct nodo* y = z->pai->pai->fe;
            if (y->cor == VERMELHO)
            {
                z->pai->cor = PRETO;
                y->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            }
            else
            {
                if (z == z->pai->fe)
                {
                    z = z->pai;
                    rotacionarDir(raiz, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacionarEsq(raiz, z);
            }
        } 
    }

    (*raiz)->cor = PRETO;
}

//retorna SENTINELA se não foi possível inserir
struct nodo* inserir(struct nodo** raiz, int chave){
    struct nodo* x = *raiz;
    struct nodo* pai_chave = *raiz;
    struct nodo* y;

    while(x->chave != SENTINELA){
        y = x;
        if (chave < x->chave)
            x = x->fe; 
        else
            x = x->fd;
    }

    struct nodo* z = buscar(*raiz, chave);
    if (z->chave == SENTINELA){
        printf("Nodo com valor %d não está na árvore\n", chave);
        return x;
    }

    z->pai = y;

    if (y->chave == SENTINELA) // se árvore está vazia
        *raiz = z;
    else if(z->chave < y->chave)
        y->fe = z;
    else
        y->fd = z;

    // depois do while x vai ser a sentinela
    z->fe = x;
    z->fd = x;
    z->cor = VERMELHO;

    corrigirInserir(raiz, z);
}

//piriri pororo
void transplantar(struct nodo** raiz, struct nodo* u, struct nodo* v){
    if (u->pai->chave == SENTINELA)
        *raiz = v;
    else if (u == u->pai->fe)
        u->pai->fe = v;
    else 
        u->pai->fd = v;
    
    v->pai = u->pai;
}

struct nodo* min_arvore(struct nodo* raiz){
    while (raiz->fe->chave != SENTINELA)
        raiz = raiz->fe;

    return raiz;
}

void corrigirExcluir(struct nodo** raiz, struct nodo* x){

}

// retorna o número de nodos excluídos
int excluir(struct nodo** raiz, int chave){
    if (!raiz) return -1;

    struct nodo* z = buscar(*raiz, chave);
    if (z->chave == SENTINELA)
    {
        printf("O nodo %d não pode ser retirado, pois não está na árvore", chave);
        return -1;
    }

    struct nodo* y = z;
    struct nodo* x;
    int corOriginal = y->cor;

    if (z->fe->chave == SENTINELA)
    {
        x = z->fd;
        transplantar(raiz, z, z->fd);
    }
    else if(z->fd->chave == SENTINELA)
    {
        x = z->fe;
        transplantar(raiz, z, z->fd);
    }
    else 
    {
        y = min_arvore(*raiz);
        corOriginal = y->cor;
        x = y->fd;
        if (y != z->fd)
        {
            transplantar(raiz, y, y->fd);
            y->fd = z->fd;
            y->fd->pai = y; 
        }
        else
            x->pai = y;

        transplantar(raiz, z, y);
        y->fe = z->fe;
        y->fe->pai = y;
        y->cor = z->cor;
    }

    if (corOriginal == PRETO)
        corrigirExcluir(raiz, x);
}

//retorna SENTINELA se não existe
// Iterativo
struct nodo* buscar(struct nodo* raiz, int chave){
    
    while (raiz->chave != SENTINELA || chave != raiz->chave)
    {
        if (chave < raiz->chave)
            raiz = raiz->fe;
        else
            raiz = raiz->fd;
    }

    return raiz;
}

// Recursivo
struct nodo* buscar_RS(struct nodo* raiz, int chave){
    if (raiz->chave != SENTINELA || chave == raiz->chave)
        return raiz;

    if (chave < raiz->chave)
        return buscar(raiz->fe, chave);
    return buscar(raiz->fd, chave);
}

void imprimirEmOrdem(struct nodo* nodo){}

void imprimirEmLargura(struct nodo* raiz){}

