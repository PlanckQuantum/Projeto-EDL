#ifndef LISTA_H
#define LISTA_H
#include "base.h"

typedef struct No {
    const Estudante *estudante;
    struct No *prox;
} No;
typedef struct {
    No *inicio;
    No *fim;
    size_t quantidade;
} Lista;

Lista *lista_criar(void);
int lista_inserir(Lista *lista, const Estudante *estudante);
const Estudante *lista_buscar(const Lista *lista, uint32_t matricula);
void lista_liberar(Lista *lista);
#endif
