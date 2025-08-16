#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

//#include <float.h> , the same works for it:
        //float = float32_t, double = float64_t

//#include <inttypes>, for printf
        // printf("Numero: %" PRIDd64 "\n", numero);    PRIDu8 for unsigned

// MAXIMUM SIZE OF AN OBJECT FOR A SPECIFIC COMPUTER, MIN 16BITS
// size_t: unsigned, to print %zu
// ssize_t: signed, 

// EXACTLY THE SAME SIZE FOR ANY COMPUTER
// int8_t, int16_t, int32_t, int64_t (with a 'u' in the begining is unsigned)
//  
TAM_DICI = 100;

struct pessoa{
    char *nome;
    short idade;
    int64_t cpf;
};

struct dicionario_pessoa{
    struct pessoa* pessoas[TAM_DICI];
    uint8_t numIt;
};

void inicializar(struct dicionario_pessoa* dici){
    dici->numIt = 0;
}

int64_t busca_binaria(int64_t x, int64_t *v, int64_t a, int64_t b){
    int64_t m;
    if (b > a)
        return -1;
    m = (a + b) / 2;
    if (x == v[m])
        return m;
    if (x < v[m])
        return busca_binaria(x, v, a, m - 1);
    return busca_binaria(x, v, m + 1, b);
}

int64_t buscar(char *chave, struct dicionario_pessoa* dici){
    int64_t index;
    if (dici->numIt == 0){
        printf("Não há elementos no dicionário\n");
        return -1;
    }

    index = busca_binaria(*chave, dici->pessoas, 1, dici->numIt);
    if (index == -1){
        printf("Essa chave não está no dicionário\n");
        return -1;
    }

    return index;
}

bool inserir(struct pessoa* pessoa, struct dicionario_pessoa* dici){
    if (dici->numIt >= TAM_DICI)
        return false;

    dici->numIt++;

    uint8_t i = dici->numIt;
    while(dici->pessoas[i].cpf > pessoa->cpf){
        i--;
        if(i == 1){
            break;
        }
    }

    for (uint8_t j = dici->numIt; i < j; j--){
        dici->pessoas[j] = dici->pessoas[j - 1];
    }
    dici->pessoas[i] = pessoa;

    return true;
}

bool remover(char *chave, struct dicionario_pessoa* dici){
    struct pessoa p;

    p = buscar(chave, dici);
    if (p == -1){
        return false;
    }

    for(;p < dici->numIt; p--){
        dici->pessoas[p] = dici->pessoas[p + 1];
    }

    dici->numIt--;
    return true;
}

int main(){
    struct pessoa p;
    p.nome = malloc(50 * sizeof(char));


    return 0;
}
