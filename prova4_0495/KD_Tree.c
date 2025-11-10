#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "KD_Tree.h"

void falhaMemoria(){
    fprintf(stderr, "Falha ao alocar memória.\n");
    exit(1);
}

void falhaScanf(){
    fprintf(stderr, "Falha ao ler.\n");
    exit(1);
}

// Troca dois valores de um vetor
void trocar(void **vet, size_t i, size_t i2)
{

	void *aux = vet[i];
	vet[i] = vet[i2];
	vet[i2] = aux;

	return;
}

// Particiona os elementos de um vetor 
size_t particionar(float **vetNodos, size_t *vetClasses, size_t coord, size_t esq, size_t dir)
{

	float *pivo = vetNodos[esq];
	size_t m = esq;

	for (size_t i = esq; i < dir; i++) {
		// Elementos menores ou iguais ao pivo ficam a esquerda do vetor
		if (*vetNodos[i] <= pivo[coord]) {
			trocar(*vetNodos, i, m);
			trocar(vetClasses, i, m);
			m++;
		}
	}

    // Pivo assume posição correta
	trocar(vetNodos, m, dir);
	trocar(&vetClasses, m, dir);

	return m;
}


void ordenarQS(float **vetNodos, size_t *vetClasses, size_t coord, size_t esq, size_t dir){
    if (esq >= dir)
		return;

	size_t pivo = particionar(vetNodos, vetClasses, coord, esq, dir);
    // Lado esquerdo do vetor
	ordenarQS(vetNodos, vetClasses, coord, esq, pivo-1);
    // Lado direito do vetor
	ordenarQS(vetNodos, vetClasses, coord, pivo+1, dir);

	return;

}

/* – Criar k-d Tree: carrega a k-d Tree da entrada padrão. Primeiro é solicitado o número N de
pontos, depois, o número de dimensões K. Finalmente, são dados todos os N pontos. Cada
ponto é seguido de um número inteiro, indicando sua classe. Juntamente com o trabalho, é
dado um exemplo de arquivo contendo uma entrada válida para a k-d Tree, que pode ser
lida da entrada padrão para criar uma árvore. A árvore não precisa ser balanceada. */

struct nodo *construir(float **vetNodos, size_t *vetClasses, size_t coord, size_t k, size_t esq, size_t dir){
    size_t meio = floor((esq+dir)/2);
    ordenarQS(vetNodos, vetClasses, coord, esq, dir);

    struct nodo *novo;
    novo->chaves = vetNodos[meio];
    novo->classe = vetClasses[meio];
    novo->fe = NULL;
    novo->fd = NULL;
    novo->numDims = k;

    if (esq <= meio)
        novo->fe = construir(vetNodos, vetClasses, (coord+1) % k, k, esq, meio-1);
    if (meio + 1 <= dir)
        novo->fd = construir(vetNodos, vetClasses, (coord+1) % k, k, meio+1, dir);

    return novo;
}

/* – Buscar: recebe um vetor de coordenadas, e retorna um ponteiro para o nodo que contém
essas coordenadas, ou NULL caso não exista. */
struct nodo* buscar(struct nodo *raiz, float *lerPonto){

    return raiz;
}

/* – z-vizinhos: recebe um vetor de coordenadas, e retorna um vetor de ponteiros de tamanho
z (z é um parâmetro), contendo os ponteiros para os z-vizinhos mais próximos do vetor. */
struct nodo** z_vizinhos(){

}