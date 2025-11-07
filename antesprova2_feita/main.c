#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "redblack.h"


#define PRETO 1
#define VERMELHO 0

extern struct nodo *NIL;

void imprimir(struct nodo *raiz){
	printf("Imprimi árvore em ordem e em largura\n");
	imprimirEmOrdem(raiz);
	printf("\n");
	imprimirEmLargura(raiz);
}

void imprimirbuscar(struct nodo* valB, int chave){
	if(valB != NIL)
	    printf("Encontrado %d.\n", valB->chave);
	else
	    printf("Nao encontrado %d.\n", chave);							
}

int testarFuncoes(struct nodo** raiz){

	// Verifica novas funcionalidades da P2
	int chave = 1;
	printf("\nTeste1: Verifica novas funcionalidades da P2\n\n");
	printf("Insere %d e %d na árvore\n", chave, chave);
	inserir(raiz, chave);
	inserir(raiz, chave);

	printf("Busca %d na árvore\n", chave);
	struct nodo* valB = buscar(*raiz, chave);
	imprimirbuscar(valB, chave);
	imprimir(*raiz);

	printf("Chama excluir uma vez para %d\n", chave);	
	if (excluir(raiz, chave) == 0) printf("Não foi possível excluir %d\n", chave);
	imprimir(*raiz);

	printf("\nTeste2: \n\n");
	printf("Insere nodos com chaves: 30 22 10 30 17 16\n");
	inserir(raiz, 30);
	inserir(raiz, 22);
	inserir(raiz, 10);
	inserir(raiz, 30);
	inserir(raiz, 17);
	inserir(raiz, 16);
	imprimir(*raiz);
	printf("Busca nodo não existente\n");
	valB = buscar(*raiz, 10000);
	imprimirbuscar(valB, 10000);
	printf("Busca nodo existente\n");
	valB = buscar(*raiz, 22);
	imprimirbuscar(valB, 22);


	printf("Exclui valor duplicado da árvore 30\n");
	if (excluir(raiz, 30) == 0) printf("Não foi possível excluir 30\n");
	imprimir(*raiz);
	
	return 0;
}

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

	printf("Digite 't' para testar todas as funções\n");

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
				puts("");
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
	
			case 't':
				testarFuncoes(&raiz);
				break;
	        default:
	            fprintf(stderr,"Opcao Invalida '%c'\n", op);
	    }
	}

	liberarArvore(&raiz);
	endNIL();
	return 0;
}
