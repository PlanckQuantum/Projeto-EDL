#ifndef LISTA_H
#define LISTA_H

#include "estudante.h"

typedef struct no {
    Estudante estudante;
    struct no *prox;
} No;

typedef struct {
    No *inicio;
    No *fim;
    int quantidade;
} Lista;

Lista *criar_lista(void);

int inserir_lista(Lista *lista, Estudante estudante);

Estudante *buscar_lista(Lista *lista, const char *matricula);

void liberar_lista(Lista *lista);

#endif
