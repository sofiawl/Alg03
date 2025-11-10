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
    if (!vetClasses) falhaMemoria();

    float **vetNodos = malloc(sizeof(size_t *) * n);
    if (!vetNodos) falhaMemoria();

    printf("Insira os pontos.\n");
    for(size_t i=0; i < n; i++){

        *vetNodos = malloc(sizeof(size_t) * k);
        if (!*vetNodos) falhaMemoria();

        for(size_t j=0; j < k; j++){
            if (!scanf("%f", &vetNodos[i][j])) falhaScanf();
        }

        if (!scanf("%ld", &vetClasses[i])) falhaScanf();
    }
    

    struct nodo raiz = *construir(vetNodos, vetClasses, 0, k, 0, n-1);
    printf("Árvore construída.\n");


    float *lerPonto = malloc(sizeof(float) * k);
    if (!lerPonto) falhaMemoria();

    while(fgets(linha, sizeof(linha), stdin)) {
	    // Remover o \n se existir
	    linha[strcspn(linha, "\n")] = '\0';


	    if(sscanf(linha, " %c", &op) != 1) {
	        fprintf(stderr, "Formato inválido\n");
	        continue;
	    }

        // finalizar e sair: f
	    if(op == 'f') break;
	
	    switch (op) {

            // buscar um nodo na árvore: b vet
	        case 'b':
	            if (sscanf(linha, "%c", &op)) {
				    fprintf(stderr, "Formato inválido\n");
				    break;
				}
                for (size_t i=0; i < k; i++){
                    if (sscanf(linha, "%f", &lerPonto[i])) {
				        fprintf(stderr, "Formato inválido\n");
				        break;
				    }                
                }

                struct nodo *b = buscar(&raiz, lerPonto);
	            if(!b)
	                fprintf(stderr,"Não encontrado.\n");
                else
	                fprintf(stderr,"Encontrado. Classe %ld.\n", b->classe);
                break;

	        // imprimir a árvore em largura: l
	        case 'l':
                /*
	            imprimirEmLargura(raiz);
	            puts("");
	            */
                break;

            // z-vizinhos: z numeroVizinhos vet	
	        case 'z':
                /*
	            */
                break;
	
	        default:
	            fprintf(stderr,"Opcao Invalida '%c'\n", op);
	    }
	}

    free(lerPonto);
    free(vetClasses);
    for (size_t i=0; i < n;i++)
        free(*vetNodos);
    free(vetNodos);
    return 0;
}