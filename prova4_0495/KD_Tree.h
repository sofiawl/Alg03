#ifndef __KD_TREE_H__
#define __KD_TREE_H__


struct nodo{
    struct nodo *fe;
    struct nodo *fd;
    size_t numDims;
    float *chaves;
    size_t classe;
};

void falhaMemoria();
void falhaScanf();
void imprimirNodo(struct nodo *nodo);
void ordenar(float **vetNodos, size_t *vetClasses, size_t coord, size_t esq, size_t dir);
struct nodo* construir(float **vetNodos, size_t *vetClasses, size_t coord, size_t k, size_t esq, size_t dir);
struct nodo* buscar(struct nodo *raiz, float *lerPonto);
struct nodo** z_vizinhos();

#endif //__KD_TREE_H__