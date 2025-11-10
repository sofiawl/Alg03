#ifndef __KD_TREE_H__
#define __KD_TREE_H__


struct nodo{
    struct nodo *fe;
    struct nodo *fd;
    size_t numDims;
    float *chaves;
    size_t classe;
};

void matarProgramaFaltaMemoria();
void falhaScanf();
void imprimirEmLargura(struct nodo *raiz);
void ordenarQS(float **vetNodos, size_t *vetClasses, size_t coord, size_t esq, size_t dir);
struct nodo* construir(float **vetNodos, size_t *vetClasses, size_t coord, size_t k, size_t esq, size_t dir);
struct nodo* buscar(struct nodo *nodo, float *lerPonto, size_t coord, size_t k);
struct nodo** z_vizinhos();

#endif //__KD_TREE_H__