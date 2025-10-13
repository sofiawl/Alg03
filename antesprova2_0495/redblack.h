#ifndef REDBLACK_H_
#define REDBLACK_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct nodo {
    int chave;
    struct nodo *fe;
    struct nodo *fd;
    struct nodo *pai;
    int cor;
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
// TESTAR E TIRAR
void imprimirEmOrdem_SR(struct nodo* nodo);

// O(n²)
void imprimirEmLargura(struct nodo* raiz);
// O(n)

//TESTAR E TIRAR
void imprimirEmLargura_R(struct nodo* raiz);

#endif//REDBLACK_H