#ifndef __KD_TREE_H__
#define __KD_TREE_H__

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

struct nodo{
    struct nodo *fe;
    struct nodo *fd;
    size_t numDims;
    float *chaves;
    size_t classe;
    float dist;
};

struct ListaZNN{
    struct nodo **vizinhos;  // Array de ponteiros para os nodos
    float *distancias;       // Distâncias correspondentes
    int tamanho;             // Quantidade atual de vizinhos
    int capMax;                   // Capacidade máxima (k vizinhos)
};

void matarProgramaFaltaMemoria();
void falhaScanf();
void imprimirEmLargura(struct nodo *raiz);
void ordenarQS(float **vetNodos, size_t *vetClasses, size_t coord, size_t esq, size_t dir);
struct nodo* construir(float **vetNodos, size_t *vetClasses, size_t coord, size_t k, size_t esq, size_t dir);
void destruirPosOrdem(struct nodo *raiz);
struct nodo* buscar(struct nodo *nodo, float *lerPonto, size_t coord, size_t k);
void  z_vizinhos(struct nodo *raiz, float *ponto, int k, int numVizinhos,
                       struct nodo **resultado, float *distancias, int *nodosEncontrados);

void imprimeZVizinhos(struct nodo **vizinhos, float *distancias, 
                      int nodosEncontrados, int k);
#endif //__KD_TREE_H__