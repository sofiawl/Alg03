#include <stdio.h>
#include <stdlib.h>

#include "redblack.h"

int main(){
	//ATENÇÃO, ESSE É APENAS UM EXEMPLO DE IMPLEMENTAÇÃO DO MAIN.
	//MODIFIQUE DE ACORDO COM SUAS NECESSIDADES E DE ACORDO COM AS ESPECIFICAÇÕES.
	struct nodo* raiz;
	
	imprimirDadosAluno();

	char op;
	int val;
	if (scanf("%c", &op) != 1) return 1;

	while(op != 'f'){

		switch (op) {
			case 'i':
				if (scanf("%d", &val) != 1) return 1;

				if(!inserir(&raiz, val))
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
				//if(valB != &SENTINELA)
				if(valB->chave != SENTINELA)
					printf("Encontrado %d\n", valB->chave);
				else
					printf("Nao encontrado %d.\n", val);
				break;
			default:
				fprintf(stderr,"Opcao Invalida %d", (int)op);
		}

		if (scanf("%c", &op) != 1) return 1;
	}

	return 0;
}