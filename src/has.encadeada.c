#include "hash_encadeada.h"
#include <stdlib.h>

static int marcar_inicial(unsigned char *bits, size_t indice) {
    unsigned char mascara = (unsigned char)(1u << (indice % 8));
    int repetido = !!(bits[indice / 8] & mascara);
    bits[indice / 8] |= mascara;
    return repetido;
}
HashEncadeada *encadeada_criar(size_t capacidade, TipoHash funcao) {
    if (!capacidade) return NULL;
    HashEncadeada *h = calloc(1, sizeof *h);
    if (!h) return NULL;
    h->capacidade = capacidade;
    h->funcao = funcao;
    h->baldes = calloc(capacidade, sizeof *h->baldes);
    h->indices_iniciais = calloc((capacidade + 7)/8, 1);
    if (!h->baldes || !h->indices_iniciais) { encadeada_liberar(h); return NULL; }
    return h;
}
int encadeada_inserir(HashEncadeada *h, const Estudante *e) {
    if (!h || !e) return 0;
    size_t indice = calcular_hash(e->matricula, h->capacidade, h->funcao);
    for (NoHash *atual = h->baldes[indice]; atual; atual = atual->prox)
        if (atual->estudante->matricula == e->matricula) return 0;
    NoHash *novo = malloc(sizeof *novo);
    if (!novo) return 0;
    novo->estudante = e;
    novo->prox = h->baldes[indice];
    h->baldes[indice] = novo;
    h->colisoes += (uint64_t)marcar_inicial(h->indices_iniciais, indice);
    h->quantidade++;
    return 1;
}
const Estudante *encadeada_buscar(const HashEncadeada *h, uint32_t matricula) {
    if (!h) return NULL;
    size_t indice = calcular_hash(matricula, h->capacidade, h->funcao);
    for (NoHash *no = h->baldes[indice]; no; no = no->prox)
        if (no->estudante->matricula == matricula) return no->estudante;
    return NULL;
}
void encadeada_liberar(HashEncadeada *h) {
    if (!h) return;
    if (h->baldes) for (size_t i = 0; i < h->capacidade; ++i) {
        NoHash *no = h->baldes[i];
        while (no) { NoHash *prox = no->prox; free(no); no = prox; }
    }
    free(h->baldes);
    free(h->indices_iniciais);
    free(h);
}
