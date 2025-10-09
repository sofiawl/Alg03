#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include "redblack.h"


#define PRETO 1
#define VERMELHO 0

struct nodo *NIL = NULL;

void matarProgramaFaltaMemoria(){
    fprintf(stderr, "Falha ao alocar memoria.\n");
    exit(1);
};

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
    retorno->grr = 20240495;

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


// Alocar e inicializar um nodo vazio
struct nodo* criarNodo(int chave){
    struct nodo* z = malloc(sizeof(struct nodo));
    if (!z){
        matarProgramaFaltaMemoria();
    }
    z->chave = chave;
    z->pai = NIL;
    z->fe = NIL;
    z->fd = NIL;
    z->cor = VERMELHO;
    return z;
}

// Rotacionar subárvore "raiz" para a esquerda 
void rotacionarEsq(struct nodo** raiz, struct nodo *x){
    if (!raiz || !x) return;

    struct nodo* y = x->fd;
    x->fd = y->fe;

    // Caso fe do fd de x não é sentinela
    if (y->fe != NIL) 
        y->fe->pai = x;

    y->pai = x->pai;

    // Caso x é a raiz
    if (x->pai == NIL) 
        *raiz = y;
    // Caso x é fe
    else if (x == x->pai->fe)
        x->pai->fe = y;
    // Caso x é fd
    else 
        x->pai->fd = y;
    
    y->fe = x;
    x->pai = y;
}


// Rotacionar subárvore "raiz" para a diretita 
void rotacionarDir(struct nodo** raiz, struct nodo* x){
    if (!raiz || !x) return;

    struct nodo* y = x->fe;
    x->fe = y->fd;

    // Caso fd do fe de x não é sentinela
    if (y->fd != NIL) 
        y->fd->pai = x;

    y->pai = x->pai;

    // Caso x é a raiz
    if (x->pai == NIL) 
        *raiz = y;
    // Caso x é fe
    else if (x == x->pai->fe)
        x->pai->fe = y;
    // Casox é fd
    else 
        x->pai->fd = y;
    

    y->fd = x;
    x->pai = y;
}

// Garantir que a Red Black mantenha suas propriedades depois de inserir novo nodo:
void corrigirInserir(struct nodo** raiz, struct nodo* z){
    if (!raiz || !z) return;

    while(z->pai->cor == VERMELHO){
        // Caso 1: pai de z é fe
        if (z->pai == z->pai->pai->fe) 
        {
            struct nodo* tio_z = z->pai->pai->fd; 
            // Caso 1.1: tio de z é vermelho
            if (tio_z->cor == VERMELHO) 
            {
                z->pai->cor  = PRETO;
                tio_z->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            }
            // Caso 1.2: tio de z é preto
            else 
            {
                // Caso z é fd
                if (z == z->pai->fd)
                {
                    z = z->pai;
                    rotacionarEsq(raiz, z);
                }

                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacionarDir(raiz, z->pai->pai);
            }
        }  
        // Caso 2: pai de z é fd
        else 
        {
            struct nodo* tio_z = z->pai->pai->fe; 
            // Caso 2.1: tio de z é vermelho
            if (tio_z->cor == VERMELHO)
            {
                z->pai->cor = PRETO;
                tio_z->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            }
            // Caso 2.2: tio de z é preto
            else
            {
                // Caso z é fe
                if (z == z->pai->fe)
                {
                    z = z->pai;
                    rotacionarDir(raiz, z);
                }

                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacionarEsq(raiz, z->pai->pai);
            }
        } 
    }

    (*raiz)->cor = PRETO;
}

// Inserir novo nodo, retornar SENTINELA se não foi possível inserir
struct nodo* inserir(struct nodo** raiz, int chave){
    if (!raiz) return NIL;

    // Evitar duplicata 
    struct nodo *buscado = buscar(*raiz, chave);
    if(buscado != NIL){
        return NIL;
    }

    struct nodo* y = NIL;
    struct nodo* x = *raiz;

    // Percorrer a raiz para buscar posição inicial do novo nodo
    while(x != NIL){
        y = x;
        // Nodos a esquerda são menores que os pais
        if(chave < x->chave)
            x = x->fe;
        // Nodos a direita são maiores que os pais
        else 
            x = x->fd;
    }

    struct nodo* z = criarNodo(chave);
    z->pai = y;

    // Caso pai do novo nodo é sentinela
    if(y == NIL)
        *raiz = z;         
    // Caso novo nodo tenha chave MENOR que seu pai
    else if(chave < y->chave)
        y->fe = z;
    // Caso novo nodo tenha chave MAIOR que seu pai
    else
        y->fd = z;


    corrigirInserir(raiz, z);

    return z;
}

// Trocar o nodo u pelo nodo v, para excluir nodos
void transplantar(struct nodo** raiz, struct nodo* u, struct nodo* v){
    if (!raiz || !u || !v) return;
    
    // Caso u é a raiz
    if (u->pai == NIL)
        *raiz = v;
    // Caso u é fe
    else if (u == u->pai->fe)
        u->pai->fe = v;
    // Caso u é fd
    else 
        u->pai->fd = v;
    
    v->pai = u->pai;
}

// Restornar o nodo com menor chave da árvore
struct nodo* min_arvore(struct nodo* raiz){
    if (!raiz) return NIL;

    // Percorrer a árvore pelos nodos à esquerda
    while (raiz->fe != NIL)
        raiz = raiz->fe;

    return raiz;
}

// Garantir que Red Black mantenha suas propriedades depois de excluir novo nodo
void corrigirExcluir(struct nodo** raiz, struct nodo* x){
    if (!raiz || !x) return;

    // W é o irmão de x
    struct nodo *w;

    while (x->pai != NIL && x->cor == PRETO ){
         // Caso 1: x é filho esquerdo
        if (x == x->pai->fe)
        {
            w = x->pai->fd;
            // Caso 1.1: w é vermelho
            if (w->cor == VERMELHO) 
            {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacionarEsq(raiz, x->pai);
                w = x->pai->fd;
            } 
            // Caso 1.2: w é preto e seus filhos são pretos      
            else if (w->fe->cor == PRETO && w->fd->cor == PRETO)
            {
                w->cor = VERMELHO;
                x = x->pai;
            }
            // Caso 1.3: w é preto e seus filhos não são pretos
            else
            {
                // Caso 1.3.1: fd de w é preto
               if (w->fd->cor == PRETO)
               {
                    w->fe->cor = PRETO;
                    w->cor = VERMELHO;
                    rotacionarDir(raiz, w);
                    w = x->pai->fd;
               } 

               w->cor = x->pai->cor;
               x->pai->cor = PRETO;
               w->fd->cor = PRETO;
               rotacionarEsq(raiz, x->pai);
               x = *raiz;
            }
        }
        // Caso 2: x é filho direito
        else 
        {
            w = x->pai->fe;
            // Caso 2.1: w é vermelho
            if (w->cor == VERMELHO) 
            {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacionarEsq(raiz, x->pai);
                w = x->pai->fe;
            } 
            // Caso 2.2: w é preto e seus filhos são pretos      
            else if (w->fe->cor == PRETO && w->fd->cor == PRETO)
            {
                w->cor = VERMELHO;
                x = x->pai;
            }
            // Caso 2.3: w é preto e seus filhos não são pretos
            else
            {
                // caso 2.3.1: fe de w é preto
               if (w->fe->cor == PRETO)
               {
                    w->fd->cor = PRETO;
                    w->cor = VERMELHO;
                    rotacionarDir(raiz, w);
                    w = x->pai->fe;
               } 

               w->cor = x->pai->cor;
               x->pai->cor = PRETO;
               w->fe->cor = PRETO;
               rotacionarEsq(raiz, x->pai);
               x = *raiz;
            }
        }
    }

    x->cor = PRETO;
}

// Excluir nodo existente na árvore e retornar o número de nodos excluídos
int excluir(struct nodo** raiz, int chave){
    if (!raiz) return -1;

    // Percorrer a árvore para verificar existência do nodo
    struct nodo* z = buscar(*raiz, chave);
    if (z == NIL)
    {
        return 0;
    }

    struct nodo* y = z;
    // X salva o filho (não sentinela) do nodo excluído para correção de excluir
    struct nodo* x;
    // Guardar cor original do nodo excluído
    int corOriginal = y->cor;

    // Caso fe de z é sentinela
    if (z->fe == NIL)
    {
        x = z->fd;
        transplantar(raiz, z, z->fd);
    }
    // Caso fd de z é sentinela
    else if(z->fd == NIL)
    {
        x = z->fe;
        transplantar(raiz, z, z->fe);
    }
    // Caso z tenha ambos filhos não sentinelas
    else 
    {
        // Descobrir o nodo com menor chave da subárvore do fd de z 
        y = min_arvore(z->fd);
        corOriginal = y->cor;
        x = y->fd;

        // Caso menor valor não é fd de z
        if (y != z->fd)
        {
            // Trocar y pelo fd de y
            transplantar(raiz, y, y->fd);
            y->fd = z->fd;
            y->fd->pai = y; 
        }
        // Caso menor valor é fd de z
        else
            x->pai = y;

        transplantar(raiz, z, y);
        y->fe = z->fe;
        y->fe->pai = y;
        y->cor = z->cor;
    }

    // Somente quando a cor inicial é preto é necessário corrigir
    if (corOriginal == PRETO)
        corrigirExcluir(raiz, x);

    free(z);
    z = NULL;
    return 1;
}

// Buscar nodo com chave "chave", restornar sentinela se não existir
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

// Versão recursiva de buscar 
struct nodo* buscar_R(struct nodo* raiz, int chave){
    if (!raiz) return NULL;

    if (raiz != NIL && chave == raiz->chave)
        return raiz;

    if (chave < raiz->chave)
        return buscar(raiz->fe, chave);
    return buscar(raiz->fd, chave);
}

// Imprimir valores das chaves em ordem (fundo esquerda, sobe, fundo direita)
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
void imprimirEmOrdem_SR(struct nodo* raiz){
    if (!raiz || raiz == NIL) return;
    
    struct nodo **pilha = malloc(1000 * sizeof(struct nodo*));
    if (!pilha){
        matarProgramaFaltaMemoria();
    }
    
    size_t topo = 0;
    struct nodo* atual = raiz;
    
    while(atual != NIL || topo > 0) {
        // Vai para o nó mais à esquerda
        while(atual != NIL) {
            pilha[topo++] = atual;
            atual = atual->fe;
        }
        
        // Visitar o nó
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
    char tipo_filho;

    if (raiz->pai == NIL)
        return 's';
    else 
        return raiz->pai->fe == raiz ? 'e' : 'd';
}

// Imprimir valores das chaves em largura (raiz, filhos raiz, netos raiz ...)
void imprimirEmLargura(struct nodo* raiz){
    if (!raiz || raiz == NIL) return;
    
    // Fila para BFS - usando array circular simples
    struct nodo **fila = malloc(1000 * sizeof(struct nodo*));
    int *niveis = malloc(1000 * sizeof(int));
    
    if (!fila || !niveis){
        matarProgramaFaltaMemoria();
    }
    
    int inicio = 0, fim = 0;
    int nivel_atual = 0;
    bool primeiro_do_nivel = true;
    
    // Adicionar raiz na fila
    fila[fim] = raiz;
    niveis[fim] = 0;
    fim++;
    
    // Enquanto fila não acabou
    while(inicio < fim) {
        struct nodo* atual = fila[inicio];
        int nivel = niveis[inicio];
        inicio++;
        
        // Se mudou de nível, imprimir nova linha
        if (nivel != nivel_atual) {
            printf("\n[%d]\t", nivel);
            nivel_atual = nivel;
            primeiro_do_nivel = false;
        } else if (primeiro_do_nivel) {
            printf("[%d]\t", nivel);
            primeiro_do_nivel = false;
        }
        
        // Imprimir o nó no formato: (COR)CHAVE [CHAVEPAI e/d]
        printf("(%c)%d ", getCor(atual), atual->chave);    
        printf("[%d%c]", atual->pai->chave, getTipoFilho(atual));
        printf("\t");

        // Adicionar primeiro filho esquerdo na fila
        if (atual->fe != NIL) {
            fila[fim] = atual->fe;
            niveis[fim] = nivel + 1;
            fim++;
        }
        
        // Adicionar depois filho direito na fila
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

// Calcular altura da Árvore
// para calcular altura preta da Árvore basta percorrer UM caminho e somar nodos número de nodos pretos
int calcularAltura(struct nodo* raiz) {
    if (!raiz || raiz == NIL) return -1;
    
    int alt_esq = calcularAltura(raiz->fe);
    int alt_dir = calcularAltura(raiz->fd);
    
    return 1 + (alt_esq > alt_dir ? alt_esq : alt_dir);
}


// Imprimir o nodo no formato: (COR)CHAVE [CHAVEPAI e/d]
void imprimirNivel(struct nodo* raiz, int nivel_alvo, int nivel_atual) {
    if (!raiz || raiz == NIL) return;
    

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

// Versão recursiva da impressão em largura
void imprimirEmLargura_R(struct nodo* raiz){
    if (!raiz) return;

    int altura = calcularAltura(raiz);
    
    // Para cada nível, chamar a função que imprime apenas esse nível
    for (int nivel = 0; nivel <= altura; nivel++) {
        printf("[%d]", nivel);
        imprimirNivel(raiz, nivel, 0);
        printf("\n");
    }
}




