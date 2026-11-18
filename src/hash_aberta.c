#include "hash_aberta.h"
#include <stdlib.h>

static int marcar_inicial(unsigned char *bits, size_t indice) {
    unsigned char mascara = (unsigned char)(1u << (indice % 8));
    int repetido = !!(bits[indice / 8] & mascara);
    bits[indice / 8] |= mascara;
    return repetido;
}
HashAberta *aberta_criar(const Base *base, size_t capacidade, TipoHash funcao) {
    if (!base || !capacidade || capacidade >= UINT32_MAX) return NULL;
    HashAberta *h = calloc(1, sizeof *h);
    if (!h) return NULL;
    h->base = base;
    h->capacidade = capacidade;
    h->funcao = funcao;
    h->posicoes = calloc(capacidade, sizeof(uint32_t));
    h->indices_iniciais = calloc((capacidade + 7)/8, 1);
    if (!h->posicoes || !h->indices_iniciais) { aberta_liberar(h); return NULL; }
    return h;
}
int aberta_inserir(HashAberta *h, size_t indice_estudante) {
    if (!h || indice_estudante >= h->base->quantidade || h->quantidade >= h->capacidade) return 0;
    uint32_t chave = h->base->estudantes[indice_estudante].matricula;
    size_t inicial = calcular_hash(chave, h->capacidade, h->funcao);
    /* Colisao = novo indice hash inicial ja produzido por outra chave. */
    h->colisoes += (uint64_t)marcar_inicial(h->indices_iniciais, inicial);
    for (size_t i = 0; i < h->capacidade; ++i) {
        size_t pos = inicial + i;
        if (pos >= h->capacidade) pos -= h->capacidade;
        uint32_t indice_mais_um = h->posicoes[pos];
        if (indice_mais_um == 0) {
            h->posicoes[pos] = (uint32_t)(indice_estudante + 1);
            h->quantidade++;
            return 1;
        }
        if (h->base->estudantes[indice_mais_um - 1].matricula == chave) return 0;
    }
    return 0;
}
const Estudante *aberta_buscar(const HashAberta *h, uint32_t matricula) {
    if (!h) return NULL;
    size_t inicial = calcular_hash(matricula, h->capacidade, h->funcao);
    for (size_t i = 0; i < h->capacidade; ++i) {
        size_t pos = inicial + i;
        if (pos >= h->capacidade) pos -= h->capacidade;
        uint32_t indice_mais_um = h->posicoes[pos];
        if (!indice_mais_um) return NULL;
        const Estudante *e = &h->base->estudantes[indice_mais_um - 1];
        if (e->matricula == matricula) return e;
    }
    return NULL;
}
void aberta_liberar(HashAberta *h) {
    if (!h) return;
    free(h->posicoes);
    free(h->indices_iniciais);
    free(h);
}
