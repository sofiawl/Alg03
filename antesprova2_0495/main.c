#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "redblack.h"


#define PRETO 1
#define VERMELHO 0

extern struct nodo *NIL;

// Inicializar sentinela global
void initNIL(void){
    NIL = malloc(sizeof(struct nodo));
    if (!NIL){
		fprintf(stderr, "Falha ao alocar memoria.\n");
    	exit(1);
    }
    
    NIL->chave = INT_MIN;
    NIL->pai = NIL->fe = NIL->fd = NIL;
    NIL->cor = PRETO;
}

// Liberar sentinela global
void endNIL(void){
    if (NIL) {
        free(NIL);
        NIL = NULL; 
    }
}

// Liberar árvore em pós ordem
void liberarArvore(struct nodo** raiz){
    if (*raiz == NIL)
        return;

    liberarArvore(&(*raiz)->fe);
    liberarArvore(&(*raiz)->fd);

    free(*raiz);

    *raiz = NULL; 
}

int main(){
	
	initNIL();
	struct nodo* raiz = NIL;
	
	imprimirDadosAluno();

	char linha[256];  
	char op;
	int val;

	while(fgets(linha, sizeof(linha), stdin)) {
	    // Remover o \n se existir
	    linha[strcspn(linha, "\n")] = '\0';
	
	    if(sscanf(linha, " %c", &op) != 1) {
	        fprintf(stderr, "Formato inválido\n");
	        continue;
	    }
	
	    if(op == 'f') break;
	
	    switch (op) {
	        case 'i':
				int n;
	            if (sscanf(linha, "%c %d%n", &op, &val, &n) != 2 || linha[n] != '\0') {
				    fprintf(stderr, "Formato inválido\n");
				    break;
				}
	            if(inserir(&raiz, val) == NIL)
	                fprintf(stderr,"Falha ao inserir.\n");
	            break;
	
	        case 'r':
	            if (sscanf(linha, "%c %d", &op, &val) != 2) {
	                fprintf(stderr, "Formato inválido para remover\n");
	                break;
	            }
	            if(!excluir(&raiz, val))
	                fprintf(stderr,"Falha ao remover %d.\n", val);
	            break;
	
	        case 'e':
	            imprimirEmOrdem(raiz);
	            puts("");
	            break;
	
	        case 'l':
	            imprimirEmLargura(raiz);
	            break;
	
	        case 'b':
	            if (sscanf(linha, "%c %d", &op, &val) != 2) {
	                fprintf(stderr, "Formato inválido para buscar\n");
	                break;
	            }
	            struct nodo* valB = buscar(raiz, val);
	            if(valB != NIL)
	                printf("Encontrado %d.\n", valB->chave);
	            else
	                printf("Nao encontrado %d.\n", val);
	            break;
	
	        default:
	            fprintf(stderr,"Opcao Invalida '%c'\n", op);
	    }
	}

	liberarArvore(&raiz);
	endNIL();
	return 0;
}
