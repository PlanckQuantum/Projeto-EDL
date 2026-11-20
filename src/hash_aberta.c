#include <stdlib.h>
#include "hash_aberta.h"

HashAberta *aberta_criar(int capacidade, int tipo) {
    HashAberta *hash;
    if (capacidade <= 0) {
        return NULL;
    }

    hash = malloc(sizeof(HashAberta));
    if (hash == NULL) {
        return NULL;
    }
    hash->capacidade = capacidade;
    hash->quantidade = 0;
    hash->colisoes = 0;
    hash->tipo = tipo;
    
    hash->tabela = calloc(capacidade, sizeof(Estudante *));
    hash->inicio_usado = calloc(capacidade, sizeof(char));
    if (hash->tabela == NULL || hash->inicio_usado == NULL) {
        aberta_liberar(hash);
        return NULL;
    }
    return hash;
}

int aberta_inserir(HashAberta *hash, Estudante *estudante) {
    int inicial;
    int posicao;
    int tentativas;

    if (hash == NULL || estudante == NULL) {
        return 0;
    }
    if (hash->quantidade == hash->capacidade) {
        return 0;
    }

    inicial = calcular_hash(estudante->matricula,
                            hash->capacidade, hash->tipo);
    posicao = inicial;
    tentativas = 0;

    while (tentativas < hash->capacidade) {
        if (hash->tabela[posicao] == NULL) {
            hash->tabela[posicao] = estudante;
            hash->quantidade++;

            if (hash->inicio_usado[inicial] == 1) {
                hash->colisoes++;
            }
            hash->inicio_usado[inicial] = 1;
            return 1;
        }
        if (hash->tabela[posicao]->matricula == estudante->matricula) {
            return 0;
        }

        posicao++;
        if (posicao == hash->capacidade) {
            posicao = 0;
        }
        tentativas++;
    }
    return 0;
}

Estudante *aberta_buscar(HashAberta *hash, int matricula) {
    int posicao;
    int tentativas;

    if (hash == NULL) {
        return NULL;
    }
    posicao = calcular_hash(matricula, hash->capacidade, hash->tipo);
    tentativas = 0;

    while (tentativas < hash->capacidade) {
        if (hash->tabela[posicao] == NULL) {
            return NULL;
        }
        if (hash->tabela[posicao]->matricula == matricula) {
            return hash->tabela[posicao];
        }

        posicao++;
        if (posicao == hash->capacidade) {
            posicao = 0;
        }
        tentativas++;
    }
    return NULL;
}

void aberta_liberar(HashAberta *hash) {
    if (hash == NULL) {
        return;
    }
    free(hash->tabela);
    free(hash->inicio_usado);
    free(hash);
}