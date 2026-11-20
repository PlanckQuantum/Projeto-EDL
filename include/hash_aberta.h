#ifndef HASH_ABERTA_H
#define HASH_ABERTA_H

#include "base.h"
#include "funcoes_hash.h"

typedef struct {
    Estudante **tabela;
    char *inicio_usado;
    int capacidade;
    int quantidade;
    int colisoes;
    int tipo;
} HashAberta;

HashAberta *aberta_criar(int capacidade, int tipo);
int aberta_inserir(HashAberta *hash, Estudante *estudante);
Estudante *aberta_buscar(HashAberta *hash, int matricula);
void aberta_liberar(HashAberta *hash);

#endif