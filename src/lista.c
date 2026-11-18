#include "lista.h"
#include <stdlib.h>

Lista *lista_criar(void) { return calloc(1, sizeof(Lista)); }
int lista_inserir(Lista *lista, const Estudante *estudante) {
    if (!lista || !estudante) return 0;
    No *novo = malloc(sizeof *novo);
    if (!novo) return 0;
    novo->estudante = estudante;
    novo->prox = NULL;
    if (lista->fim) lista->fim->prox = novo;
    else lista->inicio = novo;
    lista->fim = novo;
    lista->quantidade++;
    return 1;
}
const Estudante *lista_buscar(const Lista *lista, uint32_t matricula) {
    if (!lista) return NULL;
    for (const No *no = lista->inicio; no; no = no->prox)
        if (no->estudante->matricula == matricula) return no->estudante;
    return NULL;
}
void lista_liberar(Lista *lista) {
    if (!lista) return;
    No *no = lista->inicio;
    while (no) { No *prox = no->prox; free(no); no = prox; }
    free(lista);
}
