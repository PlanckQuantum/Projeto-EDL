#ifndef HASH_ENCADEADA_H
#define HASH_ENCADEADA_H
#include "base.h"
#include "funcoes_hash.h"

typedef struct NoHash {
    const Estudante *estudante;
    struct NoHash *prox;
} NoHash;
typedef struct {
    NoHash **baldes;
    unsigned char *indices_iniciais;
    size_t capacidade;
    size_t quantidade;
    uint64_t colisoes;
    TipoHash funcao;
} HashEncadeada;
HashEncadeada *encadeada_criar(size_t capacidade, TipoHash funcao);
int encadeada_inserir(HashEncadeada *hash, const Estudante *estudante);
const Estudante *encadeada_buscar(const HashEncadeada *hash, uint32_t matricula);
void encadeada_liberar(HashEncadeada *hash);
#endif
