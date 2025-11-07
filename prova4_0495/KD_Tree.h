#ifndef __KD_TREE_H__
#define __KD_TREE_H__


struct nodo{
    size_t numDims;
    float *chaves;
    size_t classe;
};


void imprimirNodo(struct nodo *nodo);
int inserir(struct nodo *raiz, struct nodo *novo);
struct nodo *criar(struct nodo *nodo);
struct nodo *buscar(struct nodo *raiz);
struct nodo **z_vizinhos();

#endif //__KD_TREE_H__