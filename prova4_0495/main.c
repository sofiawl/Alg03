#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "KD_Tree.h"


int main(){

    char linha[256];  
    char op;
    size_t n, k;

    // verificar quantidade mínima possível de pontos
    printf("Insira N e K.\n");
    if (scanf("%ld %ld", &n, &k) != 2){  
        fprintf(stderr, "Número inválido\n");
        return 1;
    }

    size_t *vetClasses = malloc(sizeof(size_t) * n);
    if (!vetClasses) matarProgramaFaltaMemoria();

    float **vetNodos = malloc(sizeof(float *) * n);  
    if (!vetNodos) matarProgramaFaltaMemoria();

    printf("Insira os pontos.\n");
    for(size_t i=0; i < n; i++){

        vetNodos[i] = malloc(sizeof(float) * k); 
        if (!vetNodos[i]) matarProgramaFaltaMemoria();

        for(size_t j=0; j < k; j++){
            if (scanf("%f", &vetNodos[i][j]) != 1) falhaScanf();
        }

        if (scanf("%ld", &vetClasses[i]) != 1) falhaScanf();
    }
    

    struct nodo *raiz = construir(vetNodos, vetClasses, 0, k, 0, n-1);
    printf("Árvore construída.\n");

    float *lerPonto = malloc(sizeof(float) * k);
    if (!lerPonto) matarProgramaFaltaMemoria();

    // Limpar buffer antes de ler linhas
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    while(fgets(linha, sizeof(linha), stdin)) {
        // Remover o \n
        linha[strcspn(linha, "\n")] = '\0';

        // Ler operação (com espaço antes para pular whitespace)
        if(sscanf(linha, " %c", &op) != 1) {
            fprintf(stderr, "Formato inválido\n");
            continue;
        }

        // finalizar e sair
        if(op == 'f') break;

        switch (op) {
            case 'b': {
                // b 7.2 3.0 5.8 1.6
                char *ptr = linha + 1; // pula o 'b'
                int lidos = 0;

                for (size_t i = 0; i < k; i++) {
                    int chars_consumidos;
                    if (sscanf(ptr, "%f%n", &lerPonto[i], &chars_consumidos) != 1) {
                        fprintf(stderr, "Formato inválido\n");
                        lidos = -1;
                        break;
                    }
                    ptr += chars_consumidos; // avança o ponteiro
                }

                if (lidos == -1) break;

                struct nodo *b = buscar(raiz, lerPonto, 0, k);
                if(!b)
                    printf("Não encontrado.\n");
                else
                    printf("Encontrado. Classe %ld.\n", b->classe);
                break;
            }

            case 'l':
                imprimirEmLargura(raiz);
                break;

            case 'z': {
                // z 3 5.0 2.3 3.3 1.0
                memset(lerPonto, 0, sizeof(float) * k);

                size_t numVizinhos;
                char *ptr = linha + 1; // pula o 'z'
                int chars_consumidos;

                if (sscanf(ptr, "%ld%n", &numVizinhos, &chars_consumidos) != 1) {
                    fprintf(stderr, "Formato inválido\n");
                    break;
                }
               

                ptr += chars_consumidos;

                struct nodo **vetVizinhos = malloc(sizeof(struct nodo *) * numVizinhos);
                if (!vetVizinhos) matarProgramaFaltaMemoria();

                float *vetDistancias = malloc(sizeof(float) * numVizinhos);
                if (!vetDistancias) matarProgramaFaltaMemoria();

                for (size_t i = 0; i < numVizinhos; i++) {
                    vetVizinhos[i] = NULL;
                    vetDistancias[i] = -1.0f;
                }

                int lidos = 0;
                for (size_t i = 0; i < k; i++) {
                    if (sscanf(ptr, "%f%n", &lerPonto[i], &chars_consumidos) != 1) {
                        fprintf(stderr, "Formato inválido\n");
                        lidos = -1;
                        break;
                    }
                    ptr += chars_consumidos;
                }

                if (lidos == -1) {
                    free(vetVizinhos);
                    free(vetDistancias);
                    break;
                }

                int nodosEncontrados = 0;
                z_vizinhos(raiz, lerPonto, k, numVizinhos, vetVizinhos, vetDistancias, &nodosEncontrados);
                imprimeZVizinhos(vetVizinhos, vetDistancias, nodosEncontrados, k);

                free(vetVizinhos);
                free(vetDistancias);
                break;
            }

            default:
                fprintf(stderr, "Opcao Invalida '%c'\n", op);
        }
    }

    free(lerPonto);
    free(vetClasses);
    
    for (size_t i=0; i < n; i++)
        free(vetNodos[i]);
    free(vetNodos);
    
    destruirPosOrdem(raiz);
    
    return 0;
}