#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <stdint.h>  
#include "KD_Tree.h"

// ========== FUNÇÕES DE ERRO ==========

void matar_programa_falta_memoria() {
    fprintf(stderr, "Falha ao alocar memória.\n");
    exit(EXIT_FAILURE);
}

void falha_scanf() {
    fprintf(stderr, "Falha ao ler.\n");
    exit(EXIT_FAILURE);
}

// ========== IMPRESSÃO ==========

void imprimir_nodo(const struct kd_nodo *nodo) {
    if (!nodo) {
        printf("NULL ");
        return;
    }
    
    printf("(");
    for (uint8_t i = 0; i < nodo->numDims; i++) {  
        printf("%.1f", nodo->chaves[i]);
        if (i < nodo->numDims - 1) printf(", ");
    }
    printf(", c%u) ", nodo->classe);
}

void imprimir_z_vizinhos(struct kd_nodo **vizinhos, float *distancias, uint8_t nodosEncontrados, size_t k) {
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
        printf(" (classe %d),", vizinhos[i]->classe);
        
        // Imprime a distância
        printf(" dist = %.4f\n", distancias[i]);
    }
}


void imprimir_em_largura(struct kd_nodo* raiz) {
    if (!raiz) {
        printf("Árvore vazia\n");
        return;
    }
    
    size_t cap = 128;
    struct kd_nodo **fila = malloc(cap * sizeof(struct kd_nodo*));
    if (!fila) matar_programa_falta_memoria();
    
    size_t inicio = 0, fim = 0;
    size_t nivel = 0;
    size_t nodos_nivel_atual = 1, nodos_prox_nivel = 0;
    size_t nodos_impressos_linha = 0;
    const size_t MAX_NODOS_LINHA = 7;
    
    fila[fim++] = raiz;
    printf("[%zu]\t", nivel);  // %zu para size_t
    
    while (inicio < fim) {
        struct kd_nodo* atual = fila[inicio++];
        nodos_nivel_atual--;
        
        imprimir_nodo(atual);
        nodos_impressos_linha++;
        
        if (nodos_impressos_linha >= MAX_NODOS_LINHA && nodos_nivel_atual > 0) {
            printf("\n\t");
            nodos_impressos_linha = 0;
        }
        
        if (atual->fe) {
            if (fim >= cap) {
                cap *= 2;
                fila = realloc(fila, cap * sizeof(struct kd_nodo*));
                if (!fila) matar_programa_falta_memoria();
            }
            fila[fim++] = atual->fe;
            nodos_prox_nivel++;
        }
        
        if (atual->fd) {
            if (fim >= cap) {
                cap *= 2;
                fila = realloc(fila, cap * sizeof(struct kd_nodo*));
                if (!fila) matar_programa_falta_memoria();
            }
            fila[fim++] = atual->fd;
            nodos_prox_nivel++;
        }
        
        if (nodos_nivel_atual == 0 && inicio < fim) {
            nivel++;
            printf("\n[%zu]\t", nivel);
            nodos_nivel_atual = nodos_prox_nivel;
            nodos_prox_nivel = 0;
            nodos_impressos_linha = 0;
        }
    }
    
    printf("\n");
    free(fila);
}


// ========== CONSTRUÇÃO DA KD-TREE ==========

static inline void trocar_ponteiros(float **vet, size_t i, size_t j) {
    float *aux = vet[i];
    vet[i] = vet[j];
    vet[j] = aux;
}

static inline void trocar_classes(uint32_t *vet, size_t i, size_t j) {
    uint32_t aux = vet[i];
    vet[i] = vet[j];
    vet[j] = aux;
}

static size_t particionar(float **vetNodos, uint32_t *vetClasses, 
                          uint8_t coord, size_t esq, size_t dir) {
    const float pivoValor = vetNodos[dir][coord];  
    size_t m = esq;
    
    for (size_t i = esq; i < dir; i++) {
        if (vetNodos[i][coord] <= pivoValor) {
            trocar_ponteiros(vetNodos, i, m);
            trocar_classes(vetClasses, i, m);
            m++;
        }
    }
    
    trocar_ponteiros(vetNodos, m, dir);
    trocar_classes(vetClasses, m, dir);
    return m;
}

void ordenar_quicksort(float **vetNodos, uint32_t *vetClasses, 
               uint8_t coord, size_t esq, size_t dir) {
    if (esq >= dir) return;
    
    size_t pivo = particionar(vetNodos, vetClasses, coord, esq, dir);
    
    if (pivo > 0 && esq < pivo) {
        ordenar_quicksort(vetNodos, vetClasses, coord, esq, pivo - 1);
    }
    if (pivo < dir) {
        ordenar_quicksort(vetNodos, vetClasses, coord, pivo + 1, dir);
    }
}

struct kd_nodo *construir_kdtree(float **vetNodos, uint32_t *vetClasses, 
                       uint8_t coord, uint8_t k, size_t esq, size_t dir) {
    if (esq > dir) return NULL;
    
    ordenar_quicksort(vetNodos, vetClasses, coord, esq, dir);
    
    const size_t meio = esq + (dir - esq) / 2;  // Evita overflow
    
    struct kd_nodo *novo = malloc(sizeof(struct kd_nodo));
    if (!novo) matar_programa_falta_memoria();
    
    novo->chaves = vetNodos[meio];
    novo->classe = vetClasses[meio];
    novo->numDims = k;
    novo->fe = NULL;
    novo->fd = NULL;
    
    const uint8_t proxCoord = (coord + 1 >= k) ? 0 : coord + 1;
    
    if (meio > 0 && esq <= meio - 1) {
        novo->fe = construir_kdtree(vetNodos, vetClasses, proxCoord, k, esq, meio - 1);
    }
    if (meio < dir) {
        novo->fd = construir_kdtree(vetNodos, vetClasses, proxCoord, k, meio + 1, dir);
    }
    
    return novo;
}

void destruir_pos_ordem(struct kd_nodo *raiz) {
    if (!raiz) return;
    destruir_pos_ordem(raiz->fe);
    destruir_pos_ordem(raiz->fd);
    free(raiz);  
}

// ========== BUSCA ==========

struct kd_nodo* buscar_kdnodo(struct kd_nodo *nodo, const float *lerPonto, 
                    uint8_t coord, uint8_t k) {
    if (!nodo) return NULL;
    
    // Compara todas dimensões de uma vez
    uint8_t iguais = 1;
    for (uint8_t i = 0; i < k; i++) {
        if (fabsf(nodo->chaves[i] - lerPonto[i]) > 1e-6f) {  // fabsf para float
            iguais = 0;
            break;
        }
    }
    
    if (iguais) return nodo;
    
    const uint8_t proxCoord = (coord + 1 >= k) ? 0 : coord + 1;
    
    return (lerPonto[coord] < nodo->chaves[coord]) 
        ? buscar_kdnodo(nodo->fe, lerPonto, proxCoord, k)
        : buscar_kdnodo(nodo->fd, lerPonto, proxCoord, k);
}

// ========== K-VIZINHOS MAIS PRÓXIMOS (KNN) ==========

static inline float dist_euclidiana(const struct kd_nodo *p1, const float *p2, uint8_t k) {
    float soma = 0.0f; 
    
    for (uint8_t i = 0; i < k; i++) {
        const float diff = p1->chaves[i] - p2[i];
        soma += diff * diff; 
    }
    
    return sqrtf(soma);  
}

struct kd_lista* criar_kdlista(uint8_t k, size_t numVizinhos) {
    struct kd_lista *lista = malloc(sizeof(struct kd_lista));
    if (!lista) matar_programa_falta_memoria();
    
    lista->vizinhos = malloc(numVizinhos * sizeof(struct kd_nodo*));
    lista->distancias = malloc(numVizinhos * sizeof(float));
    if (!lista->vizinhos || !lista->distancias) matar_programa_falta_memoria();
    
    lista->tamanho = 0;
    lista->capMax = numVizinhos;
    
    for (size_t i = 0; i < numVizinhos; i++) {
        lista->vizinhos[i] = NULL;
        lista->distancias[i] = FLT_MAX;
    }
    
    return lista;
}

void liberar_kdlista(struct kd_lista *lista) {
    if (!lista) return;
    free(lista->vizinhos);
    free(lista->distancias);
    free(lista);
}

static inline float maior_dist_kdlista(const struct kd_lista *lista) {
    return (lista->tamanho < lista->capMax) 
        ? FLT_MAX 
        : lista->distancias[lista->tamanho - 1];
}

static void inserir_vizinho_kdlista(struct kd_lista *lista, 
                             struct kd_nodo *candidato, float dist) {

    if (lista->tamanho >= lista->capMax && 
        dist >= lista->distancias[lista->capMax - 1]) {
        return;
    }
    
    size_t pos = (lista->tamanho < lista->capMax) 
        ? lista->tamanho++ 
        : lista->capMax - 1;
    
    while (pos > 0 && dist < lista->distancias[pos - 1]) {
        lista->distancias[pos] = lista->distancias[pos - 1];
        lista->vizinhos[pos] = lista->vizinhos[pos - 1];
        pos--;
    }
    
    lista->distancias[pos] = dist;
    lista->vizinhos[pos] = candidato;
}

static void buscar_kdlista(struct kd_nodo *r, const float *ponto, 
                     struct kd_lista *lista, uint8_t k, uint8_t coord) {
    if (!r) return;
    
    const float dist = dist_euclidiana(r, ponto, k);
    inserir_vizinho_kdlista(lista, r, dist);
    
    if (!r->fe && !r->fd) return;  // Folha
    
    const float diff = ponto[coord] - r->chaves[coord];
    struct kd_nodo *prim = (diff < 0) ? r->fe : r->fd;
    struct kd_nodo *sec = (diff < 0) ? r->fd : r->fe;
    
    const uint8_t proxCoord = (coord + 1 >= k) ? 0 : coord + 1;
    
    // Explora lado mais próximo primeiro
    if (prim) {
        buscar_kdlista(prim, ponto, lista, k, proxCoord);
    }
    
    // PODA: só explora outro lado se necessário
    if (sec) {
        const float distHiperplano = fabsf(diff);
        if (distHiperplano < maior_dist_kdlista(lista)) {
            buscar_kdlista(sec, ponto, lista, k, proxCoord);
        }
    }
}

// ========== INTERFACE PÚBLICA ==========

void z_vizinhos(struct kd_nodo *raiz, const float *ponto, uint8_t k, 
                size_t numVizinhos, struct kd_nodo **resultado, 
                float *distancias, size_t *numEncontrados) {
    
    struct kd_lista *lista = criar_kdlista(k, numVizinhos);
    buscar_kdlista(raiz, ponto, lista, k, 0);
    
    *numEncontrados = lista->tamanho;
    
    for (size_t i = 0; i < lista->tamanho; i++) {
        resultado[i] = lista->vizinhos[i];
        distancias[i] = lista->distancias[i];
    }
    
    liberar_kdlista(lista);
}
