#include <stdlib.h>
#include "hash_encadeada.h"

HashEncadeada *encadeada_criar(int capacidade, int tipo) {
    HashEncadeada *hash;
    if (capacidade <= 0) {
        return NULL;
    }

    hash = malloc(sizeof(HashEncadeada));
    if (hash == NULL) {
        return NULL;
    }
    hash->capacidade = capacidade;
    hash->quantidade = 0;
    hash->colisoes = 0;
    hash->tipo = tipo;

    hash->baldes = calloc(capacidade, sizeof(NoHash *));
    if (hash->baldes == NULL) {
        free(hash);
        return NULL;
    }
    return hash;
}

int encadeada_inserir(HashEncadeada *hash, Estudante *estudante) {
    int posicao;
    NoHash *atual;
    NoHash *novo;

    if (hash == NULL || estudante == NULL) {
        return 0;
    }
    posicao = calcular_hash(estudante->matricula, hash->capacidade, hash->tipo);
    atual = hash->baldes[posicao];
    while (atual != NULL) {
        if (atual->estudante->matricula == estudante->matricula) {
            return 0;
        }
        atual = atual->prox;
    }

    novo = malloc(sizeof(NoHash));
    if (novo == NULL) {
        return 0;
    }
    novo->estudante = estudante;

    if (hash->baldes[posicao] != NULL) {
        hash->colisoes++;
    }

    novo->prox = hash->baldes[posicao];
    hash->baldes[posicao] = novo;
    hash->quantidade++;
    return 1;
}

Estudante *encadeada_buscar(HashEncadeada *hash, int matricula) {
    int posicao;
    NoHash *atual;

    if (hash == NULL) {
        return NULL;
    }
    posicao = calcular_hash(matricula, hash->capacidade, hash->tipo);
    atual = hash->baldes[posicao];

    while (atual != NULL) {
        if (atual->estudante->matricula == matricula) {
            return atual->estudante;
        }
        atual = atual->prox;
    }
    return NULL;
}

void encadeada_liberar(HashEncadeada *hash) {
    int i;
    NoHash *atual;
    NoHash *proximo;

    if (hash == NULL) {
        return;
    }
    for (i = 0; i < hash->capacidade; i++) {
        atual = hash->baldes[i];
        while (atual != NULL) {
            proximo = atual->prox;
            free(atual);
            atual = proximo;
        }
    }
    free(hash->baldes);
    free(hash);
}