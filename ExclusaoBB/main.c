#include <stdio.h>
#include <stdlib.h>

struct nodoBB{
    int key;
    struct nodoBB *ls;
    struct nodoBB *rs;
    struct nodoBB *dad;
};

// Substitui uma subárvore por outra, a substituída fica sem nada
void transplantar(struct nodoBB **src, struct nodoBB *old, struct nodoBB *new){
    if (old->dad == NULL)
        *src = new;
    else if(old == old->dad->ls)
        old->dad->ls = new;
    else 
        old->dad->rs = new;

    if (new != NULL)
        new->dad = old->dad;
    
    return;
}

struct nodoBB* min_nodo(struct nodoBB *src){
    if (src)
        return NULL;

    struct nodoBB *aux = src;
    while(aux){
        aux = aux->ls; 
    }

    return aux;
}

// Excluir o nodo rm da árvore
void excluir(struct nodoBB *src, struct nodoBB *rm){
    if (rm->ls == NULL){
        transplantar(&src, rm, rm->rs);
        return;
    }
    if (rm->rs == NULL){
        transplantar(&src, rm, rm->ls);
        return;
    }

    struct nodoBB *min = min_nodo(src);
    if (!min)
        return;

    if (min != rm->rs){
        transplantar(&src, rm, rm->rs);
        min->rs = rm->rs;
        min->rs->dad = min;
    }
    transplantar(&src, rm, min);
    min->ls = rm->ls;
    min->ls->dad = min;

    free(rm);
    return;
}

void inserir(struct nodoBB *src, struct nodoBB *new){
    if (!src || !new)
        return;
    
    struct nodoBB *aux = src;
    while(aux){
        if (new->key > aux->key){
            if (!aux->rs){
                aux->rs = new;
                new->dad = aux;
                printf("Inseri nodo %d como filho direito de %d\n", new->key, aux->key);
                return;
            }
            
            aux = aux->rs;
        }
        else {
            if (!aux->ls){
                aux->ls = new;
                new->dad = aux;
                printf("Inseri nodo %d como filho esquerdo de %d\n", new->key, aux->key);
                return;
            }

            aux = aux->ls;
        }
    }
}

struct nodoBB* buscar_DFS(struct nodoBB *no, int x){
    if (no == NULL)
        return NULL;

    if (no->key == x)
        return no;

    struct nodoBB *retorno = buscar_DFS(no->ls, x);
    if (retorno)
        return retorno;

    retorno = buscar_DFS(no->rs, x);

    return retorno;
}

void preordem(struct nodoBB *no){
	if (no != NULL){
		printf("%d ", no->key);
		preordem(no->ls);
		preordem(no->rs);
	}
}

int main(){
    
    int n;
    printf("Digite a quantidade de nodos da sua árvore:\n");
    scanf("%d", &n);
    if (n <= 0){
        printf("Número inválido\n Abortando operação...\n");
        return 0;
    }

    int chave;
    printf("Digite o valor da raiz:\n");
    scanf("%d", &chave);
    struct nodoBB *src = malloc(sizeof(struct nodoBB));
    if (!src){return 1;}
    src->key = chave;
    src->dad = NULL;
    src->ls = NULL;
    src->rs = NULL;
    
    if (n == 1){
        printf("Só há um nodo em sua árvore %d", chave);
    }
    
    struct nodoBB *nodo;
    printf("Digite os valores de cada nodo tirando a raiz (Neste padrão: 1 2 3 ...):\n");
    for(int i = 1; i < n; i++){
        scanf("%d", &chave);
        nodo = malloc(sizeof(struct nodoBB));
        if (!nodo){return 1;}
        nodo->key = chave;
        nodo->dad = nodo->ls = nodo->rs = NULL;
        inserir(src, nodo);
    }

    printf("\nSua árvore ordenada e lida em pré ordem é:\n");
    preordem(src);
    printf("\n");

    int rm;
    printf("\nInforme o nodo que deseja excluir:\n");
    scanf("%d", &rm);
    
    struct nodoBB *nodo_rm = buscar_DFS(src, rm);
    if (nodo_rm->key != rm){
        printf("Erro na busca, nodo achado foi %d\n",nodo->key);
    }

    excluir(src, nodo_rm);

    printf("\nSua árvore sem %d e lida em pré ordem é:\n", rm);
    preordem(src);
    printf("\n");

    return 0;
}