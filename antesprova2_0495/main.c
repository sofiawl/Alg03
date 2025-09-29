#include <stdio.h>
#include <stdlib.h>
#include "redblack.h"


// SEMPRE QUE ATUALIZAR RAIZ ATUALIZAR FILHO DE NIL

// ESTÁ CRIANDO DOIS NODOS A MAIS NA INSERÇÃO, UM SLA PORQUE O OUTRO QUANDO INSERE VALORES IGUAIS
struct nodo *NIL = NULL;

int main(){
	
	initNIL();
	struct nodo* raiz = NIL;

	imprimirDadosAluno();

	char linha[256];  
	char op;
	int val;

	while(fgets(linha, sizeof(linha), stdin)) {
	    // Remove o \n se existir
	    linha[strcspn(linha, "\n")] = '\0';
	
	    if(sscanf(linha, "%c", &op) != 1) {
	        fprintf(stderr, "Formato inválido\n");
	        continue;
	    }
	
	    if(op == 'f') break;
	
	    switch (op) {
	        case 'i':
	            if (sscanf(linha, "%c %d", &op, &val) != 2) {
	                fprintf(stderr, "Formato inválido para inserir\n");
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
	            imprimirEmLargura_RS(raiz);
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

/*
	char op;
	int val;
	if (scanf("%c", &op) != 1) return 1;

	while(op != 'f'){

		switch (op) {
			case 'i':
				if (scanf("%d", &val) != 1) return 1;
				
				if(inserir(&raiz, val) == NIL)
					fprintf(stderr,"Falha ao inserir.\n");
				break;
			case 'r':
				if (scanf("%d", &val) != 1) return 1;
				if(!excluir(&raiz, val))
					fprintf(stderr,"Falha ao remover %d.\n", val);
				break;
			case 'e':
				imprimirEmOrdem(raiz);
				puts("\n");
				break;
			case 'l':
				imprimirEmLargura(raiz);
				break;
			case 'b':
			 	if (scanf("%d", &val) != 1) return 1;
				struct nodo* valB = buscar(raiz, val);
				if(valB != NIL)
					printf("Encontrado %d\n", valB->chave);
				else
					printf("Nao encontrado %d.\n", val);
				break;
			default:
				fprintf(stderr,"Opcao Invalida %d", (int)op);
		}

		if (scanf("%c", &op) != 1) return 1;
	}
*/