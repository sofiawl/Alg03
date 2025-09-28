#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#define TAM_NOME 3

struct nodo{
	char nome[TAM_NOME];
	short idade;
	int64_t cpf;
	struct nodo *fe;
	struct nodo *fd;
};

void printpessoa(struct nodo no){
	printf("\nNome:");
	for (size_t i = 0; i < TAM_NOME; i++){
		printf("%c", no.nome[i]);
	}

	/*printf("\nIdade: %d", no.idade);
	printf("\nCPF: %ld", no.cpf);*/
}

void preordem(struct nodo *no){
	if (no != NULL){
		printpessoa(*no);
		preordem(no->fe);
		preordem(no->fd);
	}
}

void preordemSR(struct nodo *no){
	struct nodo **pilha = malloc(1048576 * sizeof(struct nodo));
	size_t topo = 0;

	printpessoa(*no);
	pilha[topo++] = no->fd;
	pilha[topo++] = no->fe;

	while(topo > 0) {
		struct nodo *aux = pilha[--topo];
		if (aux){
			printpessoa(*aux);
			pilha[topo++] = aux->fd;
			pilha[topo++] = aux->fe;
		}
	} 

	free(pilha);
}

void emordem(struct nodo *no){
	if (no != NULL){
		emordem(no->fe);
		printpessoa(*no);
		emordem(no->fd);
	}
}

void emordemSR(struct nodo *no){
	struct nodo **pilha = malloc(1048576 * sizeof(struct nodo));
	size_t topo = 0;

	pilha[topo++] = no;
	pilha[topo++] = no->fd;
	pilha[topo++] = no->fe;

	while(topo > 0) {
		struct nodo *aux = pilha[--topo];
		if (aux){
			pilha[topo++] = aux->fd;
			pilha[topo++] = aux->fe;
		}
		
	} 

	free(pilha);
}

void posordem(struct nodo *no){
	if (no != NULL){
		posordem(no->fe);
		posordem(no->fd);
		printpessoa(*no);
	}
}

int64_t altura(struct nodo *no){
	if (no == NULL)
		return -1;

	int64_t altE, altD;
	
	altE = altura(no->fe);
	altD = altura(no->fd);

	if (altE > altD)
		return altE+1;
	else 
		return altD+1;	
}

int insereNodo(struct nodo *raiz, struct nodo *new){
	if (!raiz) return -1;
	if (!raiz->fe){
		raiz->fe = new;
		return 0;
	} else if (!raiz->fd){
		raiz->fd = new;
		return 0;
	}	else
		return -1;
}

void infosNodo(struct nodo *raiz){
	raiz->nome[0] = 'a' + rand() % 26;
	raiz->nome[1] = 'a' + rand() % 26;
	raiz->nome[2] = '\0';

	raiz->idade = rand() % 101;
	raiz->cpf = 10000000000LL + rand() % 90000000000LL;
    /*printf("Nome: ");
    fgets(raiz->nome, sizeof(raiz->nome), stdin);
    raiz->nome[strcspn(raiz->nome, "\n")] = '\0'; // remove o \n

    printf("Idade: ");
    scanf("%hd", &raiz->idade);
    while (getchar() != '\n'); // limpa buffer

    printf("CPF: ");
    scanf("%ld", &raiz->cpf);
    while (getchar() != '\n'); // limpa buffer*/
}

int main(){
	srand(time(NULL));

	struct nodo *raiz = malloc(sizeof(struct nodo));
	if (!raiz) return 1;

	struct nodo *filhoE = malloc(sizeof(struct nodo));
	if (!filhoE) return 1;

	struct nodo *filhoD = malloc(sizeof(struct nodo));
	if (!filhoD) return 1;

	struct nodo *netoEum = malloc(sizeof(struct nodo));
	if (!netoEum) return 1;

	struct nodo *netoEdois = malloc(sizeof(struct nodo));
	if (!netoEdois) return 1;

	infosNodo(raiz);
	printpessoa(*raiz);
	infosNodo(filhoE);
	printpessoa(*filhoE);
	infosNodo(filhoD);
	printpessoa(*filhoD);
	infosNodo(netoEum);
	printpessoa(*netoEum);
	infosNodo(netoEdois);
	printpessoa(*netoEdois);
	insereNodo(raiz, filhoE);
	insereNodo(raiz, filhoD);
	insereNodo(filhoE, netoEum);
	insereNodo(filhoE, netoEdois);
	
	printf("\n");
	int64_t h = altura(raiz);
	printf("A altura da árvore é: %ld\n", h);

	printf("\nTravessia com pré-ordem: \n");
	preordem(raiz);
	printf("\n");

	printf("\nTravessia com pré-ordem SR: \n");
	preordem(raiz);
	printf("\n");

	printf("\nTravessia com em-ordem: \n");
	emordem(raiz);
	printf("\n");

	printf("\nTravessia com pós-ordem: \n");
	posordem(raiz);
	printf("\n");

	free(raiz);
	free(filhoE);
	free(filhoD);

	return 0;
}
