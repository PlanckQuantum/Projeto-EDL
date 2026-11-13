#include <stdio.h>
#include "lista.h"

int main(void) {
    Lista *lista = criar_lista();

    if (lista == NULL) {
        printf("Erro ao criar a lista!\n");
        return 1;
    }

    Estudante aluno1 = {"001", "Rodrigo", "Computacao"};
    Estudante aluno2 = {"002", "Maria", "Direito"};
    Estudante aluno3 = {"003", "Joao", "Medicina"};

    inserir_lista(lista, aluno1);
    inserir_lista(lista, aluno2);
    inserir_lista(lista, aluno3);

    printf("Total de estudantes: %d\n", lista->quantidade);

    Estudante *encontrado = buscar_lista(lista, "002");

    if (encontrado != NULL) {
        printf("Estudante encontrado!\n");
        printf("Matricula: %s\n", encontrado->matricula);
        printf("Nome: %s\n", encontrado->nome);
        printf("Curso: %s\n", encontrado->curso);
    } else {
        printf("Estudante nao encontrado.\n");
    }

    liberar_lista(lista);

    return 0;
}
