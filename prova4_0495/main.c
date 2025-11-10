#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "KD_Tree.h"


int main(){

	char linha[256];  
	char op;
    size_t n, k;

    // verificar quantidade mínhima possível de pontos
    printf("Insira N e K.\n");
    if (scanf("%ld %ld", &n, &k) != 1){
        fprintf(stderr, "Número inválido\n");
    }

    size_t *vetClasses = malloc(sizeof(size_t) * n);
    if (!vetClasses) matarProgramaFaltaMemoria();

    float **vetNodos = malloc(sizeof(size_t *) * n);
    if (!vetNodos) matarProgramaFaltaMemoria();

    printf("Insira os pontos.\n");
    for(size_t i=0; i < n; i++){

        *vetNodos = malloc(sizeof(size_t) * k);
        if (!*vetNodos) matarProgramaFaltaMemoria();

        for(size_t j=0; j < k; j++){
            if (!scanf("%f", &vetNodos[i][j])) falhaScanf();
        }

        if (!scanf("%ld", &vetClasses[i])) falhaScanf();
    }
    

    struct nodo *raiz = construir(vetNodos, vetClasses, 0, k, 0, n-1);
    printf("Árvore construída.\n");

    float *lerPonto = malloc(sizeof(float) * k);
    if (!lerPonto) matarProgramaFaltaMemoria();

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
                // imprimirEmLargura(raiz);
                break;

            case 'z': {
                // z 3 5.0 2.3 3.3 1.0
                int numVizinhos;
                char *ptr = linha + 1; // pula o 'z'
                int chars_consumidos;

                if (sscanf(ptr, "%d%n", &numVizinhos, &chars_consumidos) != 1) {
                    fprintf(stderr, "Formato inválido\n");
                    break;
                }
                ptr += chars_consumidos;

                int lidos = 0;
                for (size_t i = 0; i < k; i++) {
                    if (sscanf(ptr, "%f%n", &lerPonto[i], &chars_consumidos) != 1) {
                        fprintf(stderr, "Formato inválido\n");
                        lidos = -1;
                        break;
                    }
                    ptr += chars_consumidos;
                }

                if (lidos == -1) break;

                // zVizinhos(raiz, lerPonto, numVizinhos, k);
                break;
            }

            default:
                fprintf(stderr, "Opcao Invalida '%c'\n", op);
        }
    }

    free(lerPonto);
    
    free(vetClasses);
    for (size_t i=0; i < n;i++)
        free(*vetNodos);
    free(vetNodos);
    return 0;
}