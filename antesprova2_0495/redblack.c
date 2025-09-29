#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include "redblack.h"


void matarProgramaFaltaMemoria(){};

struct aluno* getAluno(){
    struct aluno* retorno = malloc(sizeof(struct aluno));
    if(!retorno)
        matarProgramaFaltaMemoria();
    retorno->nome = malloc(sizeof("SOFIA WAMSER LIMA"));//sizeof conta o \0
	if(!retorno->nome)
		matarProgramaFaltaMemoria();
    retorno->nomeDinf = malloc(sizeof("swl24"));
    if(!retorno->nomeDinf)
        matarProgramaFaltaMemoria();
    
	strcpy(retorno->nome, "SOFIA WAMSER LIMA");
    strcpy(retorno->nomeDinf, "swl24");
    retorno->grr = 495;

	return retorno;
}

void imprimirDadosAluno(){
    struct aluno* aluno = getAluno();
    
    printf("Trabalho de %s\n", aluno->nome);
    printf("Login Dinf %s\n", aluno->nomeDinf);
    printf("GRR %0u\n\n", aluno->grr);

    free(aluno->nome);
    free(aluno->nomeDinf);
    free(aluno);

    return;
}

// inicializa o sentinela
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

void endNIL(void){
    if (NIL) {
        free(NIL);
        NIL = NULL; 
    }
}

// libera árvore em pós ordem
void liberarArvore(struct nodo** raiz){
    if (*raiz == NIL)
        return;

    liberarArvore(&(*raiz)->fe);
    liberarArvore(&(*raiz)->fd);

    free(*raiz);

    *raiz = NULL; 
}

struct nodo* criarNodo(int chave){
    printf("nodo criado\n");
    struct nodo* z = malloc(sizeof(struct nodo));
    if (!z){
        fprintf(stderr, "Falha ao alocar memoria.\n");
        exit(1);
    }
    z->chave = chave;
    z->pai = NIL;
    z->fe = NIL;
    z->fd = NIL;
    z->cor = VERMELHO;
    return z;
}

void rotacionarEsq(struct nodo** raiz, struct nodo *x){
    if (!raiz || !x) return;

    // devo garantir que x nem y são sentinelas
    struct nodo* y = x->fd;
    x->fd = y->fe;

    if (y->fe != NIL) // se fe não é sentinela
        y->fe->pai = x;

    y->pai = x->pai;

    if (x->pai == NIL) // se x for a raiz
        *raiz = y;
    else if (x == x->pai->fe)
        x->pai->fe = y;
    else 
        x->pai->fd = y;
    

    y->fe = x;
    x->pai = y;
}

void rotacionarDir(struct nodo** raiz, struct nodo* x){
    if (!raiz || !x) return;

    // devo garantir que x nem y são sentinelas
    struct nodo* y = x->fe;
    x->fe = y->fd;

    if (y->fd != NIL) // se fd não é sentinela 
        y->fd->pai = x;

    y->pai = x->pai;

    if (x->pai == NIL) // se x for a raiz
        *raiz = y;
    else if (x == x->pai->fe)
        x->pai->fe = y;
    else 
        x->pai->fd = y;
    

    y->fd = x;
    x->pai = y;
}

void corrigirInserir(struct nodo** raiz, struct nodo* z){
    if (!raiz || !z) return;

    while(z->pai->cor == VERMELHO){
        if (z->pai == z->pai->pai->fe) // pai de z é filho esquerdo
        {
            struct nodo* tio_z = z->pai->pai->fd; // y é tio de z
            if (tio_z->cor == PRETO) // pai e tio de z não vermelhor
            {
                z->pai->cor  = PRETO;
                tio_z->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            }
            else
            {
                // caso 1
                if (z == z->pai->fd)
                {
                    z = z->pai;
                    rotacionarEsq(raiz, z);
                }
                // caso 2
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacionarDir(raiz, z->pai->pai);
            }
        }  
        else // pai de z é filho direito
        {
            struct nodo* y = z->pai->pai->fe;
            if (y->cor == VERMELHO)
            {
                z->pai->cor = PRETO;
                y->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            }
            else
            {
                if (z == z->pai->fe)
                {
                    z = z->pai;
                    rotacionarDir(raiz, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacionarEsq(raiz, z);
            }
        } 
    }

    (*raiz)->cor = PRETO;
}

// retorna SENTINELA se não foi possível inserir
struct nodo* inserir(struct nodo** raiz, int chave){
    if (!raiz) return NIL;

    // evita duplicata 
    struct nodo *buscado = buscar(*raiz, chave);
    printf("buscado chave: %d\n", buscado->chave);
    if(buscado != NIL){
        printf("duplicado %d\n", chave);
        return NIL;
    }

    struct nodo* y = NIL;
    struct nodo* x = *raiz;

    while(x != NIL){
        y = x;
        if(chave < x->chave) x = x->fe;
        else x = x->fd;
    }

    struct nodo* z = criarNodo(chave);
    z->pai = y;

    if(y == NIL)
        *raiz = z;         
    else if(chave < y->chave)
        y->fe = z;
    else
        y->fd = z;

    // filhos já são NIL por novoNodo 
    corrigirInserir(raiz, z);
    // verificar o que retornar
    return z;
}


void transplantar(struct nodo** raiz, struct nodo* u, struct nodo* v){
    if (!raiz || !u || !v) return;
    
    if (u->pai == NIL)
        *raiz = v;
    else if (u == u->pai->fe)
        u->pai->fe = v;
    else 
        u->pai->fd = v;
    
    v->pai = u->pai;
}

struct nodo* min_arvore(struct nodo* raiz){
    if (!raiz) return NIL;

    while (raiz->fe != NIL)
        raiz = raiz->fe;

    return raiz;
}

void corrigirExcluir(struct nodo** raiz, struct nodo* x){
    if (!raiz || !x) return;

    // w é o irmão de x
    struct nodo *w;

    while (x->pai != NIL && x->cor == PRETO ){
         // se x é filho esquerdo
        if (x == x->pai->fe)
        {
            w = x->pai->fd;
            // caso 01: irmão de x é vermelho
            if (w->cor == VERMELHO) 
            {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacionarEsq(raiz, x->pai);
                w = x->pai->fd;
            } 
            // caso 02: w é preto e seus filhos são pretos      
            else if (w->fe->cor == PRETO && w->fd->cor == PRETO)
            {
                w->cor = VERMELHO;
                x = x->pai;
            }
            else
            {
                // caso 03: w é preto, w->fe é vermelho e w->fd é preto
               if (w->fd->cor == PRETO)
               {
                    w->fe->cor = PRETO;
                    w->cor = VERMELHO;
                    rotacionarDir(raiz, w);
                    w = x->pai->fd;
               } 
               // caso 04: w é preto e w->fd é vermelho
               w->cor = x->pai->cor;
               x->pai->cor = PRETO;
               w->fd->cor = PRETO;
               rotacionarEsq(raiz, x->pai);
               x = *raiz;
            }
        }
        // se x é filho direito
        else 
        {
            w = x->pai->fe;
            // caso 01: irmão de x é vermelho
            if (w->cor == VERMELHO) 
            {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacionarEsq(raiz, x->pai);
                w = x->pai->fe;
            } 
            // caso 02: w é preto e seus filhos são pretos      
            else if (w->fe->cor == PRETO && w->fd->cor == PRETO)
            {
                w->cor = VERMELHO;
                x = x->pai;
            }
            else
            {
                // caso 03: w é preto, w->fe é preto e w->fd é vermelho
               if (w->fe->cor == PRETO)
               {
                    w->fd->cor = PRETO;
                    w->cor = VERMELHO;
                    rotacionarDir(raiz, w);
                    w = x->pai->fe;
               } 
               // caso 04: w é preto e w->fe é vermelho
               w->cor = x->pai->cor;
               x->pai->cor = PRETO;
               w->fe->cor = PRETO;
               rotacionarEsq(raiz, x->pai);
               x = *raiz;
            }
        }
    }
}

// retorna o número de nodos excluídos
int excluir(struct nodo** raiz, int chave){
    if (!raiz) return -1;

    struct nodo* z = buscar(*raiz, chave);
    if (z == NIL)
    {
        printf("O nodo %d não pode ser retirado, pois não está na árvore", chave);
        return -1;
    }

    struct nodo* y = z;
    struct nodo* x;
    int corOriginal = y->cor;

    if (z->fe == NIL)
    {
        x = z->fd;
        transplantar(raiz, z, z->fd);
    }
    else if(z->fd == NIL)
    {
        x = z->fe;
        transplantar(raiz, z, z->fd);
    }
    else 
    {
        y = min_arvore(*raiz);
        corOriginal = y->cor;
        x = y->fd;
        if (y != z->fd)
        {
            transplantar(raiz, y, y->fd);
            y->fd = z->fd;
            y->fd->pai = y; 
        }
        else
            x->pai = y;

        transplantar(raiz, z, y);
        y->fe = z->fe;
        y->fe->pai = y;
        y->cor = z->cor;
    }

    if (corOriginal == PRETO)
        corrigirExcluir(raiz, x);

    free(z);
    z = NULL;
}

//retorna SENTINELA se não existe
// Iterativo
struct nodo* buscar(struct nodo* raiz, int chave){
    if (!raiz) return NULL;

    while (raiz != NIL && chave != raiz->chave)
    {
        if (chave < raiz->chave)
            raiz = raiz->fe;
        else
            raiz = raiz->fd;
    }

    return raiz;
}

// Recursivo
struct nodo* buscar_RS(struct nodo* raiz, int chave){
    if (!raiz) return NULL;

    if (raiz != NIL && chave == raiz->chave)
        return raiz;

    if (chave < raiz->chave)
        return buscar(raiz->fe, chave);
    return buscar(raiz->fd, chave);
}

void imprimirEmOrdem(struct nodo* nodo){
    if (!nodo || nodo == NIL) return;

    if (nodo != NIL)
    {
        imprimirEmOrdem(nodo->fe);
        printf("%d\t", nodo->chave);
        imprimirEmOrdem(nodo->fd);
    }

}

// Versão iterativa da impressão em ordem
void imprimirEmOrdem_RS(struct nodo* raiz){
    if (!raiz || raiz == NIL) return;
    
    struct nodo **pilha = malloc(1000 * sizeof(struct nodo*));
    if (!pilha){
        fprintf(stderr, "Falha ao alocar memoria.\n");
        exit(1);
    }
    
    size_t topo = 0;
    struct nodo* atual = raiz;
    
    while(atual != NIL || topo > 0) {
        // Vai para o nó mais à esquerda
        while(atual != NIL) {
            pilha[topo++] = atual;
            atual = atual->fe;
        }
        
        // Visita o nó
        atual = pilha[--topo];
        printf("%d\t", atual->chave);
        
        // Vai para a subárvore direita
        atual = atual->fd;
    }
    
    free(pilha);
}

char getCor(struct nodo* raiz){
    return raiz->cor == PRETO ? 'B' : 'R';
}

char getTipoFilho(struct nodo* raiz){
    if (raiz->pai == NIL)
        return 's';
    else 
        return raiz->pai->fe == raiz ? 'e' : 'd';
}

void imprimirNivel(struct nodo* raiz, int nivel_alvo, int nivel_atual) {
    if (!raiz || raiz == NIL) return;
    
    // Se chegou no nível desejado, imprime
    if (nivel_atual == nivel_alvo) {
        printf("(%c)%d\t", getCor(raiz), raiz->chave);
        printf(" [%d%c]", raiz->pai->chave, getTipoFilho(raiz));
        
        return;
    }
    
    if (raiz->fe != NIL)
        imprimirNivel(raiz->fe, nivel_alvo, nivel_atual + 1);
    if (raiz->fd != NIL)
        imprimirNivel(raiz->fd, nivel_alvo, nivel_atual + 1);
}

int calcularAltura(struct nodo* raiz) {
    if (!raiz || raiz == NIL) return -1;
    
    int alt_esq = calcularAltura(raiz->fe);
    int alt_dir = calcularAltura(raiz->fd);
    
    return 1 + (alt_esq > alt_dir ? alt_esq : alt_dir);
}

void imprimirEmLargura(struct nodo* raiz){
    if (!raiz) return;

    int altura = calcularAltura(raiz);
    
    // Para cada nível, chama a função que imprime apenas esse nível
    for (int nivel = 0; nivel <= altura; nivel++) {
        printf("[%d]", nivel);
        imprimirNivel(raiz, nivel, 0);
        printf("\n");
    }
}

// Impressão em largura com formato especificado
void imprimirEmLargura_RS(struct nodo* raiz){
    if (!raiz || raiz == NIL) return;
    
    // Fila para BFS - usando array circular simples
    struct nodo **fila = malloc(1000 * sizeof(struct nodo*));
    int *niveis = malloc(1000 * sizeof(int));
    
    if (!fila || !niveis){
        fprintf(stderr, "Falha ao alocar memoria.\n");
        exit(1);
    }
    
    int inicio = 0, fim = 0;
    int nivel_atual = 0;
    bool primeiro_do_nivel = true;
    
    // Adiciona raiz na fila
    fila[fim] = raiz;
    niveis[fim] = 0;
    fim++;
    
    while(inicio < fim) {
        struct nodo* atual = fila[inicio];
        int nivel = niveis[inicio];
        inicio++;
        
        // Se mudou de nível, imprime nova linha
        if (nivel != nivel_atual) {
            printf("\n[%d]\t", nivel);
            nivel_atual = nivel;
            primeiro_do_nivel = false;
        } else if (primeiro_do_nivel) {
            printf("[%d]\t", nivel);
            primeiro_do_nivel = false;
        }
        
        // Imprime o nó no formato: (COR)CHAVE [CHAVEPAI e/d]
        printf("(%c)%d ", getCor(atual), atual->chave);    
        printf("[%d%c]", atual->pai->chave, getTipoFilho(atual));
        printf("\t");

        // Adiciona filhos na fila
        if (atual->fe != NIL) {
            fila[fim] = atual->fe;
            niveis[fim] = nivel + 1;
            fim++;
        }
        
        if (atual->fd != NIL) {
            fila[fim] = atual->fd;
            niveis[fim] = nivel + 1;
            fim++;
        }
    }
    
    printf("\n");
    free(fila);
    free(niveis);
}
