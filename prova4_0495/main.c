#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "KD_Tree.h"

int main(void) {
    char linha[256];  
    char op;
    size_t n;
    uint8_t k;  

    // Lê parâmetros iniciais
    printf("Insira N e K.\n");
    
    unsigned long n_temp;
    unsigned int k_temp;
    if (scanf("%lu %u", &n_temp, &k_temp) != 2) {
        fprintf(stderr, "Número inválido\n");
        return EXIT_FAILURE;
    }
    
    // Validação de limites
    if (k_temp > 255) {
        fprintf(stderr, "Erro: K deve ser ≤ 255\n");
        return EXIT_FAILURE;
    }
    if (n_temp == 0) {
        fprintf(stderr, "Erro: N deve ser > 0\n");
        return EXIT_FAILURE;
    }
    
    n = (size_t)n_temp;
    k = (uint8_t)k_temp;

    uint32_t *vetClasses = malloc(n * sizeof(uint32_t));
    if (!vetClasses) matar_programa_falta_memoria();

    float **vetNodos = malloc(n * sizeof(float*));
    if (!vetNodos) {
        free(vetClasses); 
        matar_programa_falta_memoria();
    }

    printf("Insira os pontos.\n");
    for (size_t i = 0; i < n; i++) {
        vetNodos[i] = malloc(k * sizeof(float));
        if (!vetNodos[i]) {
            for (size_t j = 0; j < i; j++) {
                free(vetNodos[j]);
            }
            free(vetNodos);
            free(vetClasses);
            matar_programa_falta_memoria();
        }

        // Lê coordenadas
        for (uint8_t j = 0; j < k; j++) {  
            if (scanf("%f", &vetNodos[i][j]) != 1) {
                falha_scanf();
            }
        }

        if (scanf("%u", &vetClasses[i]) != 1) {
            falha_scanf();
        }
    }

    // Constrói a árvore
    struct kd_nodo *raiz = construir_kdtree(vetNodos, vetClasses, 0, k, 0, n - 1);
    printf("Árvore construída.\n");

    // Buffer para leitura de pontos
    float *lerPonto = malloc(k * sizeof(float));
    if (!lerPonto) matar_programa_falta_memoria();

    // Limpa buffer de entrada
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    // Loop principal de comandos
    while (fgets(linha, sizeof(linha), stdin)) {
        // Remove newline
        linha[strcspn(linha, "\n")] = '\0';

        // Lê operação
        if (sscanf(linha, " %c", &op) != 1) {
            fprintf(stderr, "Formato inválido\n");
            continue;
        }

        // Finalizar
        if (op == 'f') break;

        switch (op) {
            case 'b': {  
                // Buscar ponto exato
                char *ptr = linha + 1;  // Pula 'b'

                // Lê coordenadas
                int valido = 1;
                for (uint8_t i = 0; i < k; i++) { 
                    int chars_consumidos;
                    if (sscanf(ptr, "%f%n", &lerPonto[i], &chars_consumidos) != 1) {
                        fprintf(stderr, "Formato inválido na coordenada %u\n", i);
                        valido = 0;
                        break;
                    }
                    ptr += chars_consumidos;
                }

                if (!valido) break;

                struct kd_nodo *resultado = buscar_kdnodo(raiz, lerPonto, 0, k);
                if (!resultado) {
                    printf("Não encontrado.\n");
                } else {
                    printf("Encontrado. Classe %u.\n", resultado->classe); 
                }
                break;
            }

            case 'l':  
                // Listar árvore
                imprimir_em_largura(raiz);
                break;

            case 'z': {  
                // Z-vizinhos (KNN)
                char *ptr = linha + 1;  // Pula 'z'
                int chars_consumidos;

                // Lê número de vizinhos
                unsigned long numVizinhos_temp;
                if (sscanf(ptr, "%lu%n", &numVizinhos_temp, &chars_consumidos) != 1) {
                    fprintf(stderr, "Formato inválido: número de vizinhos\n");
                    break;
                }
                
                size_t numVizinhos = (size_t)numVizinhos_temp;  
                
                if (numVizinhos == 0) {
                    fprintf(stderr, "Erro: número de vizinhos deve ser > 0\n");
                    break;
                }
                if (numVizinhos > n) {
                    fprintf(stderr, "Aviso: pediu %zu vizinhos mas há apenas %zu pontos\n", 
                            numVizinhos, n);
                    numVizinhos = n;
                }

                ptr += chars_consumidos;

                // Aloca arrays de resultado
                struct kd_nodo **vetVizinhos = malloc(numVizinhos * sizeof(struct kd_nodo*));
                if (!vetVizinhos) matar_programa_falta_memoria();

                float *vetDistancias = malloc(numVizinhos * sizeof(float));
                if (!vetDistancias) {
                    free(vetVizinhos);
                    matar_programa_falta_memoria();
                }

                // Lê coordenadas do ponto de consulta
                int valido = 1;
                for (uint8_t i = 0; i < k; i++) {  
                    if (sscanf(ptr, "%f%n", &lerPonto[i], &chars_consumidos) != 1) {
                        fprintf(stderr, "Formato inválido na coordenada %u\n", i);
                        valido = 0;
                        break;
                    }
                    ptr += chars_consumidos;
                }

                if (!valido) {
                    free(vetVizinhos);
                    free(vetDistancias);
                    break;
                }

                // Executa busca KNN
                size_t nodosEncontrados;  
                z_vizinhos(raiz, lerPonto, k, numVizinhos, 
                          vetVizinhos, vetDistancias, &nodosEncontrados);
                
                imprimir_z_vizinhos(vetVizinhos, vetDistancias, nodosEncontrados, k);

                // Libera memória
                free(vetVizinhos);
                free(vetDistancias);
                break;
            }

            default:
                fprintf(stderr, "Opção inválida '%c'\n", op);
                fprintf(stderr, "Comandos válidos: b (buscar), l (listar), z (k-nn), f (finalizar)\n");
        }
    }

    // ===== CLEANUP FINAL =====

    free(lerPonto);
    free(vetClasses);
    
    for (size_t i = 0; i < n; i++) {
        free(vetNodos[i]);
    }
    free(vetNodos);
    
    destruir_pos_ordem(raiz);
    
    return 0;
}