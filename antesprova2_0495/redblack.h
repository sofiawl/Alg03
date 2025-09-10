#ifndef REDBLACK_H_
#define REDBLACK_H_

#define SENTINELA INT_MIN

#define PRETO 1
#define VERMELHO 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct nodo {
    int chave;
    struct nodo *fe;
    struct nodo *fd;
    struct nodo *pai;
    unsigned int cor;
};

struct aluno{
    char* nome;//nome completo
    char* nomeDinf;//nome de usuário nas servidoras do dinf
    unsigned int grr;
};

struct aluno* getAluno();

void imprimirDadosAluno();

//retorna SENTINELA se não foi possível inserir
struct nodo* inserir(struct nodo** raiz, int chave);

// retorna o número de nodos excluídos
int excluir(struct nodo** raiz, int chave);

//retorna SENTINELA se não existe
struct nodo* buscar(struct nodo* raiz, int chave);

void imprimirEmOrdem(struct nodo* nodo);

void imprimirEmLargura(struct nodo* raiz);

#endif//REDBLACK_H