#include <stdlib.h>
#include "lista.h"

Lista *lista_criar(void) {
    Lista *lista = malloc(sizeof(Lista));
    if (lista == NULL) {
        return NULL;
    }
    lista->inicio = NULL;
    lista->fim = NULL;
    lista->quantidade = 0;
    return lista;
}

int lista_inserir(Lista *lista, Estudante *estudante) {
    No *novo;
    if (lista == NULL || estudante == NULL) {
        return 0;
    }

    novo = malloc(sizeof(No));
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

Estudante *lista_buscar(Lista *lista, int matricula) {
    No *atual;
    if (lista == NULL) {
        return NULL;
    }

    atual = lista->inicio;
    while (atual != NULL) {
        if (atual->estudante->matricula == matricula) {
            return atual->estudante;
        }
        atual = atual->prox;
    }
    return NULL;
}

void lista_liberar(Lista *lista) {
    No *atual;
    No *proximo;
    if (lista == NULL) {
        return;
    }

    atual = lista->inicio;
    while (atual != NULL) {
        proximo = atual->prox;
        free(atual);
        atual = proximo;
    }
    free(lista);
}
