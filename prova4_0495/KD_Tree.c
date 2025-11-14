#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include "KD_Tree.h"

void matarProgramaFaltaMemoria(){
    fprintf(stderr, "Falha ao alocar memória.\n");
    exit(1);
}

void falhaScanf(){
    fprintf(stderr, "Falha ao ler.\n");
    exit(1);
}

/**
 * Imprime nodo com classe
 */
void imprimirNodo(struct nodo *nodo) {
    if (!nodo) {
        printf("NULL ");
        return;
    }
    
    printf("(");
    for (size_t i = 0; i < nodo->numDims; i++) {
        printf("%.1f", nodo->chaves[i]);
        if (i < nodo->numDims - 1) {
            printf(", ");
        }
    }
    printf(", c%ld) ", nodo->classe);
}


void realocaFila(struct nodo ***fila, int *cap) {
    *cap *= 2;
    *fila = realloc(*fila, (*cap) * sizeof(struct nodo*));
    if (!*fila) {
        fprintf(stderr, "Erro ao realocar fila\n");
        exit(1);
    }
}

void imprimirEmLargura(struct nodo* raiz) {
    if (!raiz) {
        printf("Árvore vazia\n");
        return;
    }
    
    int cap = 128;
    struct nodo **fila = malloc(cap * sizeof(struct nodo*));
    if (!fila) {
        fprintf(stderr, "Erro ao alocar fila\n");
        exit(1);
    }
    
    int inicio = 0, fim = 0;
    int nivel = 0;
    int nodos_nivel_atual = 1, nodos_prox_nivel = 0;
    
    fila[fim++] = raiz;
    printf("[%d]\t", nivel);
    
    while (inicio < fim) {
        struct nodo* atual = fila[inicio++];
        nodos_nivel_atual--;
        
        imprimirNodo(atual);
        
        if (atual->fe != NULL) {
            if (fim >= cap) realocaFila(&fila, &cap);
            fila[fim++] = atual->fe;
            nodos_prox_nivel++;
        }
        
        if (atual->fd != NULL) {
            if (fim >= cap) realocaFila(&fila, &cap);
            fila[fim++] = atual->fd;
            nodos_prox_nivel++;
        }
        
        if (nodos_nivel_atual == 0 && inicio < fim) {
            nivel++;
            printf("\n[%d]\t", nivel);
            nodos_nivel_atual = nodos_prox_nivel;
            nodos_prox_nivel = 0;
        }
    }
    
    printf("\n");
    free(fila);
}

void imprimeZVizinhos(struct nodo **vizinhos, float *distancias, 
                      int nodosEncontrados, int k) {
    printf("Vizinhos mais próximos:\n");
    
    for (int i = 0; i < nodosEncontrados; i++) {
        // Imprime as coordenadas
        for (int j = 0; j < k; j++) {
            printf("%.1f", vizinhos[i]->chaves[j]);
            if (j < k - 1) {
                printf(", ");
            }
        }
        
        // Imprime a classe (supondo que existe um campo 'classe' no struct nodo)
        printf(" (classe %ld),", vizinhos[i]->classe);
        
        // Imprime a distância
        printf(" dist = %.4f\n", distancias[i]);
    }
}


// ========== FUNÇÕES AUXILIARES ==========

// Troca dois ponteiros em um vetor de ponteiros
void trocarPonteiros(float **vet, size_t i, size_t j) {
    float *aux = vet[i];
    vet[i] = vet[j];
    vet[j] = aux;
}

// Troca dois valores em um vetor de size_t
void trocarClasses(size_t *vet, size_t i, size_t j) {
    size_t aux = vet[i];
    vet[i] = vet[j];
    vet[j] = aux;
}

// ========== QUICKSORT ==========

size_t particionar(float **vetNodos, size_t *vetClasses, size_t coord, size_t esq, size_t dir) {
    // Escolhe o último elemento como pivô
    float pivoValor = vetNodos[dir][coord];  
    size_t m = esq;
    
    for (size_t i = esq; i < dir; i++) {
        // Elementos menores ou iguais ao pivô ficam à esquerda
        if (vetNodos[i][coord] <= pivoValor) {  
            trocarPonteiros(vetNodos, i, m);
            trocarClasses(vetClasses, i, m);
            m++;
        }
    }
    
    // Pivô assume posição correta
    trocarPonteiros(vetNodos, m, dir);
    trocarClasses(vetClasses, m, dir);
    
    return m;
}

void ordenarQS(float **vetNodos, size_t *vetClasses, size_t coord, size_t esq, size_t dir) {
    if (esq >= dir)
        return;
    
    size_t pivo = particionar(vetNodos, vetClasses, coord, esq, dir);
    
    // Lado esquerdo (cuidado com underflow!)
    if (pivo > 0 && esq < pivo) {  
        ordenarQS(vetNodos, vetClasses, coord, esq, pivo - 1);
    }
    
    // Lado direito
    if (pivo < dir) {  
        ordenarQS(vetNodos, vetClasses, coord, pivo + 1, dir);
    }
}

// ========== CONSTRUÇÃO DA KD-TREE ==========

struct nodo *construir(float **vetNodos, size_t *vetClasses, size_t coord, 
                       size_t k, size_t esq, size_t dir) {
    
    // Caso base: intervalo inválido
    if (esq > dir) {
        return NULL;
    }
    
    // Ordena pela coordenada atual
    ordenarQS(vetNodos, vetClasses, coord, esq, dir);
    
    // Encontra o elemento do meio
    size_t meio = esq + (dir - esq) / 2; 
    
    // Cria novo nodo
    struct nodo *novo = malloc(sizeof(struct nodo)); 
    if (!novo) {
        fprintf(stderr, "Erro ao alocar memória para nodo\n");
        exit(1);
    }
    
    // Inicializa o nodo
    novo->chaves = vetNodos[meio];
    novo->classe = vetClasses[meio];
    novo->numDims = k;
    novo->fe = NULL;
    novo->fd = NULL;
    
    // Constrói subárvore esquerda
    if (meio > 0 && esq <= meio - 1) { 
        novo->fe = construir(vetNodos, vetClasses, (coord + 1) % k, k, esq, meio - 1);
    }
    
    // Constrói subárvore direita
    if (meio < dir) { 
        novo->fd = construir(vetNodos, vetClasses, (coord + 1) % k, k, meio + 1, dir);
    }
    
    return novo;
}

void destruirPosOrdem(struct nodo *raiz) {
    if (raiz != NULL) {
        destruirPosOrdem(raiz->fe);
        destruirPosOrdem(raiz->fd);
        // Nota: NÃO libera raiz->chaves aqui, pois aponta para vetNodos original
        free(raiz);
    }
}


// ========== BUSCAR DA KD-TREE ==========

/* – Buscar: recebe um vetor de coordenadas, e retorna um ponteiro para o nodo que contém
essas coordenadas, ou NULL caso não exista. */
/* – Buscar: recebe um vetor de coordenadas, e retorna um ponteiro para o nodo que contém
   essas coordenadas, ou NULL caso não exista. */
struct nodo* buscar(struct nodo *nodo, float *lerPonto, size_t coord, size_t k) {
    // Caso base: nodo NULL
    if (!nodo) 
        return NULL;
    
    int iguais = 1;  // Assume que são iguais
    for (size_t i = 0; i < k; i++) {
        // Compara com tolerância para floats (importante!)
        if (fabs(nodo->chaves[i] - lerPonto[i]) > 0.0001) {
            iguais = 0;
            break;
        }
    }
    
    // Se encontrou o ponto exato, retorna
    if (iguais) 
        return nodo;
    
    // Continua a busca recursiva baseado na coordenada atual
    if (lerPonto[coord] < nodo->chaves[coord])
        return buscar(nodo->fe, lerPonto, (coord+1)%k, k);
    else
        return buscar(nodo->fd, lerPonto, (coord+1)%k, k);
}


// ========== ALGORITMO KNN NA KD-TREE ==========


/* – z-vizinhos: recebe um vetor de coordenadas, e retorna um vetor de ponteiros de tamanho
z (z é um parâmetro), contendo os ponteiros para os z-vizinhos mais próximos do vetor. */
float distCart(struct nodo p1, float *p2, int k){
    float res = 0.0;
    float soma = 0.0;

    for (size_t i=0; i<k; i++){
        soma += p1.chaves[i] - p2[i];
        soma *= soma;
        res += soma;
    }

    return sqrt(res);
}

// Inicializa a lista de k vizinhos
struct ListaZNN* criaListaZNN(int k, int numVizinhos) {

    struct ListaZNN *lista = malloc(sizeof(struct ListaZNN));
    if (!lista) matarProgramaFaltaMemoria();
    
    lista->vizinhos = malloc(numVizinhos * sizeof(struct nodo*));
    lista->distancias = malloc(numVizinhos * sizeof(float));
    if (!lista->vizinhos || !lista->distancias) matarProgramaFaltaMemoria();

    lista->tamanho = 0;
    lista->capMax = numVizinhos;
    
    // Inicializa com distâncias infinitas
    for (int i = 0; i < numVizinhos; i++) {
        lista->vizinhos[i] = NULL;
        lista->distancias[i] = FLT_MAX;
    }
    
    return lista;
}

// Libera a memória da lista
void liberaListaZNN(struct ListaZNN *lista) {
    free(lista->vizinhos);
    free(lista->distancias);
    free(lista);
}

// Retorna a maior distância atual (distância do k-ésimo vizinho)
// Usado para poda da busca
float maiorDistanciaZNN(struct ListaZNN *lista) {
    if (lista->tamanho < lista->capMax) {
        return FLT_MAX;  // Ainda não temos k vizinhos
    }
    return lista->distancias[lista->tamanho - 1];
}

// Insere um vizinho mantendo a lista ordenada por distância
// Mantém apenas os k melhores
void insereVizinhoZNN(struct ListaZNN *lista, struct nodo *candidato, float dist) {
    // Se a lista está cheia E a nova distância não é melhor, descarta
    if (lista->tamanho >= lista->capMax && dist >= lista->distancias[lista->capMax - 1]) {
        return;
    }
    
    // Determina posição de inserção
    int pos = lista->tamanho;
    
    // Se a lista está cheia, vai substituir o último
    if (lista->tamanho >= lista->capMax) {
        pos = lista->capMax - 1;
    } else {
        lista->tamanho++;
    }
    
    // "Insertion sort": desloca elementos maiores para direita
    while (pos > 0 && dist < lista->distancias[pos - 1]) {
        lista->distancias[pos] = lista->distancias[pos - 1];
        lista->vizinhos[pos] = lista->vizinhos[pos - 1];
        pos--;
    }
    
    // Insere na posição correta
    lista->distancias[pos] = dist;
    lista->vizinhos[pos] = candidato;
}


void buscaZNN(struct nodo *r, float *ponto, struct ListaZNN *lista, int k, int coord) {
    if (!r) return;
    
    // Calcula distância do nodo atual ao ponto de consulta
    
    float dist = distCart(*r, ponto, k);
    
    // Tenta inserir o nodo atual na lista dos k melhores
    insereVizinhoZNN(lista, r, dist);
    
    // Se é folha, não há mais nada a explorar
    if (!r->fe && !r->fd) {
        return;
    }
    
    // 1. Escolhe qual subárvore explorar primeiro baseado na coordenada atual
    // 2. Explora a subárvore "mais próxima" primeiro
    // 3. Usa PODA: só explora a outra subárvore se ela pode conter vizinhos melhores
    
    struct nodo *prim, *sec;
    float diff = ponto[coord] - r->chaves[coord];
    
    if (diff < 0) {
        prim = r->fe;  // Ponto está à esquerda do hiperplano
        sec = r->fd;
    } else {
        prim = r->fd;  // Ponto está à direita do hiperplano
        sec = r->fe;
    }
    
    // Explora a subárvore mais próxima primeiro
    int proxCoord = (coord + 1) % k;
    if (prim) {
        buscaZNN(prim, ponto, lista, k, proxCoord);
    }
    
    // ===== PODA (Backtracking) =====
    // Só explora a outra subárvore se:
    // 1. Ela existe, E
    // 2. A distância ao hiperplano é menor que a distância do k-ésimo vizinho
    // 
    // Isso evita explorar ramos que não podem conter vizinhos melhores
    if (sec) {
        float distHiperplano = fabs(diff);
        float piorDistancia = maiorDistanciaZNN(lista);
        
        if (distHiperplano < piorDistancia) {
            buscaZNN(sec, ponto, lista, k, proxCoord);
        }
    }
}

void imprimirLista(struct ListaZNN *lista){
    printf("VIZINHOS\n");
    for (int i=0; i<lista->tamanho; i++){
        imprimirNodo(lista->vizinhos[i]);
    }
    printf("\n");

    printf("DISTANCIAS\n");
    for (int i=0; i<lista->tamanho; i++)
        printf("%f, ", lista->distancias[i]);

    printf("\n");
    printf("tamanho: %d\n", lista->tamanho);
    printf("capacidade: %d\n", lista->capMax);
}
/**
 * Encontra os k vizinhos mais próximos de um ponto
 * Retorna os vizinhos em um array ordenado por distância
 */
void  z_vizinhos(struct nodo *raiz, float *ponto, int k, int numVizinhos,
                       struct nodo **resultado, float *distancias, int *numEncontrados) {

    // Cria lista temporária para a busca
    struct ListaZNN *lista = criaListaZNN(k, numVizinhos);

    // Executa a busca KNN
    buscaZNN(raiz, ponto, lista, k, 0);
    
    *numEncontrados = lista->tamanho;
    // Copia resultados para os arrays de saída
    for (int i = 0; i < lista->tamanho; i++) {
        resultado[i] = lista->vizinhos[i];
        distancias[i] = lista->distancias[i];
    }
    
    // Libera memória temporária
    liberaListaZNN(lista);
}
