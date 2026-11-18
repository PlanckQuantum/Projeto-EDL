#ifndef HASH_ABERTA_H
#define HASH_ABERTA_H
#include "base.h"
#include "funcoes_hash.h"

typedef struct {
    const Base *base;
    uint32_t *posicoes; 
    unsigned char *indices_iniciais; 
    size_t capacidade;
    size_t quantidade;
    uint64_t colisoes;
    TipoHash funcao;
} HashAberta;
HashAberta *aberta_criar(const Base *base, size_t capacidade, TipoHash funcao);
int aberta_inserir(HashAberta *hash, size_t indice_estudante);
const Estudante *aberta_buscar(const HashAberta *hash, uint32_t matricula);
void aberta_liberar(HashAberta *hash);
#endif
