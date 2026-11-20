#ifndef LISTA_H
#define LISTA_H

#include "base.h"

typedef struct No {
    Estudante *estudante;
    struct No *prox;
} No;

typedef struct {
    No *inicio;
    No *fim;
    int quantidade;
} Lista;

Lista *lista_criar(void);
int lista_inserir(Lista *lista, Estudante *estudante);
Estudante *lista_buscar(Lista *lista, int matricula);
void lista_liberar(Lista *lista);

#endif
