#include <stdio.h>
#include <stdlib.h>
#include "KD_Tree.h"


int main(){

	char linha[256];  
	char op;
    int n, k;

    // verificar quantidade mínhima possível de pontos
    printf("Insira N e K.\n");
    if (sscanf("%d %d", n, k) != 1){
        fprintf(stderr, "Número inválido\n");
    }

    struct nodo raiz;
    raiz.numDims = k;
    raiz.chaves = malloc(sizeof(float) * raiz.numDims);
    if (!raiz.chaves){
        fprintf(stderr, "Falha ao alocar memória.\n");
        exit(1);
    }

    // TALVEZ SEJA CRIAR KD_TREE
    // enquanto pontos são escritos construir nodos na árvore
    printf("Insira os pontos.\n");
    for(size_t i=0; i < n; i++){
        // criar nodo

        for(size_t j=0; j < k; j++){
            // ler pontos do nodo no vetor de chave 
        }

        //ler classe do nodo  

        // inserir nodo
    }
    // de criar kd_tree retornar 0, então imprimir
    printf("Árvore construída.\n");


    float *lerPontos = malloc(sizeof(float) * k);

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
                for (size_t i=0; i<k; i++){
                    if (sscanf(linha, "%f", &lerPontos[i])) {
				        fprintf(stderr, "Formato inválido\n");
				        break;
				    }                
                }

                struct nodo *b = buscar(struct nodo* raiz);
	            if(!b)
	                fprintf(stderr,"Não encontrado.\n");
                else{
	                fprintf(stderr,"Encontrado. Classe %d.\n", b->classe);
                    // imprimir chavesa do ponto
                }
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

    return 0;
}