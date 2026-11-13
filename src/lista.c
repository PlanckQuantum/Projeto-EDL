#include <stdlib.h>
#include <string.h>

#include "lista.h"

Lista *criar_lista(void) {
    Lista *lista = malloc(sizeof(Lista));

    if (lista == NULL) {
        return NULL;
    }

    lista->inicio = NULL;
    lista->fim = NULL;
    lista->quantidade = 0;

    return lista;
}

int inserir_lista(Lista *lista, Estudante estudante) {
    if (lista == NULL) {
        return 0;
    }

    No *novo = malloc(sizeof(No));

    if (novo == NULL) {
        return 0;
    }

    novo->estudante = estudante;
    novo->prox = NULL;

    if (lista->inicio == NULL) {
        lista->inicio = novo;
    } else {
        lista->fim->prox = novo;
    }

    lista->fim = novo;
    lista->quantidade++;

    return 1;
}
a
Estudante *buscar_lista(Lista *lista, const char *matricula) {
    if (lista == NULL || matricula == NULL) {
        return NULL;
    }

    No *atual = lista->inicio;

    while (atual != NULL) {
        if (strcmp(atual->estudante.matricula, matricula) == 0) {
            return &atual->estudante;
        }

        atual = atual->prox;
    }

    return NULL;
}

void liberar_lista(Lista *lista) {
    if (lista == NULL) {
        return;
    }

    No *atual = lista->inicio;

    while (atual != NULL) {
        No *proximo = atual->prox;
        free(atual);
        atual = proximo;
    }

    free(lista);
}
