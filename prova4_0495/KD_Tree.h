#ifndef __KD_TREE_H__
#define __KD_TREE_H__

#include <stdint.h>   
#include <stddef.h>   

// ========== ESTRUTURAS ==========

struct kd_nodo {
    float *chaves;         
    uint32_t classe; 
    uint8_t numDims;        
    struct kd_nodo *fe;        
    struct kd_nodo *fd;        
};

struct kd_lista {
    struct kd_nodo **vizinhos;
    float *distancias;
    size_t tamanho;
    size_t capMax;           
};

// ========== FUNÇÕES DE ERRO ==========

void matar_programa_falta_memoria();

void falha_scanf();

// ========== FUNÇÕES DE UTILIDADE ==========

void imprimir_nodo(const struct kd_nodo *nodo);

void imprimir_em_largura(struct kd_nodo* raiz);

void imprimir_z_vizinhos(struct kd_nodo **vizinhos, float *distancias, uint8_t nodosEncontrados, size_t k);


// ========== CONSTRUÇÃO DA KD-TREE ==========

struct kd_nodo *construir_kdtree(float **vetNodos, uint32_t *vetClasses, 
                       uint8_t coord, uint8_t k, size_t esq, size_t dir);

void destruir_pos_ordem(struct kd_nodo *raiz);

// ========== BUSCA ==========

struct kd_nodo* buscar_kdnodo(struct kd_nodo *nodo, const float *lerPonto, uint8_t coord, uint8_t k);

// ========== K-VIZINHOS MAIS PRÓXIMOS (KNN) ==========

struct kd_lista* criar_kdlista(uint8_t k, size_t numVizinhos);

void liberar_kdlista(struct kd_lista *lista);

void z_vizinhos(struct kd_nodo *raiz, const float *ponto, uint8_t k, 
                size_t numVizinhos, struct kd_nodo **resultado, 
                float *distancias, size_t *numEncontrados);

// ========== FUNÇÕES INTERNAS (QUICKSORT) ==========

void ordenar_quicksort(float **vetNodos, uint32_t *vetClasses, uint8_t coord, size_t esq, size_t dir);

// ========== NOTAS DE IMPLEMENTAÇÃO ==========

/*
 * ESCOLHA DE TIPOS - RESUMO:
 * 
 * 1. uint8_t (1 byte):
 *    - Dimensões (k): raramente >255, economiza memória e cache
 *    - Coordenadas atuais: mesmo range que dimensões
 * 
 * 2. uint32_t (4 bytes):
 *    - Classes: suporta 4+ bilhões de classes diferentes
 *    - Usa metade da memória de size_t em sistemas 64-bit
 * 
 * 3. size_t (4 ou 8 bytes dependendo da arquitetura):
 *    - Índices de arrays: tipo correto por definição
 *    - Tamanhos e capacidades: garante compatibilidade com malloc/sizeof
 *    - Número de elementos: tipo seguro para qualquer tamanho de array
 * 
 * 4. float (4 bytes):
 *    - Coordenadas e distâncias: precisão suficiente para a maioria dos casos
 *    - 2x mais rápido que double em muitas CPUs
 *    - Melhor uso de cache (cabe mais dados por linha de cache)
 * 
 * 5. const em ponteiros:
 *    - Indica que a função não modifica os dados apontados
 *    - Permite otimizações do compilador
 *    - Previne bugs de modificação acidental
 * 
 * ECONOMIA DE MEMÓRIA:
 * - Antes: sizeof(struct nodo) ≈ 48 bytes (64-bit)
 * - Depois: sizeof(struct nodo) ≈ 37 bytes (64-bit)
 * - Economia: ~23% de memória por nodo!
 * 
 */

#endif //__KD_TREE_H__