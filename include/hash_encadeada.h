#ifndef HASH_ENCADEADA_H
#define HASH_ENCADEADA_H

#include "base.h"
#include "funcoes_hash.h"

typedef struct NoHash {
    Estudante *estudante;
    struct NoHash *prox;
} NoHash;

typedef struct {
    NoHash **baldes;
    int capacidade;
    int quantidade;
    int colisoes;
    int tipo;
} HashEncadeada;

HashEncadeada *encadeada_criar(int capacidade, int tipo);

int encadeada_inserir(HashEncadeada *hash, Estudante *estudante);

Estudante *encadeada_buscar(HashEncadeada *hash, int matricula);

void encadeada_liberar(HashEncadeada *hash);

#endif