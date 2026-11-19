#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"

int validar_linha(char linha[], int *matricula) {
    int tamanho = strlen(linha);
    int i;
    int numero = 0;

    while (tamanho > 0 &&
           (linha[tamanho - 1] == '\n' || linha[tamanho - 1] == '\r')) {
        tamanho--;
        linha[tamanho] = '\0';
    }

    if (tamanho < 12) {
        return 0;
    }

    for (i = 0; i < 8; i++) {
        if (linha[i] < '0' || linha[i] > '9') {
            return 0;
        }
        numero = numero * 10 + (linha[i] - '0');
    }

    if (linha[8] != ';') {
        return 0;
    }

    i = 9;
    while (linha[i] != '\0' && linha[i] != ';') {
        i++;
    }
    if (i == 9 || linha[i] != ';' || linha[i + 1] == '\0') {
        return 0;
    }

    i++;
    while (linha[i] != '\0') {
        if (linha[i] == ';') {
            return 0;
        }
        i++;
    }

    *matricula = numero;
    return 1;
}

int base_carregar(Base *base, char *caminho, int limite) {
    FILE *arquivo;
    char linha[512];
    char *novo_texto;
    int capacidade_texto = 1024 * 1024;
    int tamanho;
    int matricula;

    if (base == NULL || caminho == NULL || limite <= 0) {
        return 0;
    }

    base->estudantes = NULL;
    base->texto = NULL;
    base->quantidade = 0;
    base->texto_usado = 0;

    arquivo = fopen(caminho, "r");
    if (arquivo == NULL) {
        printf("Nao foi possivel abrir o arquivo: %s\n", caminho);
        return 0;
    }

    base->estudantes = malloc(limite * sizeof(Estudante));
    base->texto = malloc(capacidade_texto);
    if (base->estudantes == NULL || base->texto == NULL) {
        printf("Faltou memoria para carregar os registros.\n");
        fclose(arquivo);
        base_liberar(base);
        return 0;
    }

    while (base->quantidade < limite && fgets(linha, 512, arquivo) != NULL) {
        tamanho = strlen(linha);
        if (tamanho == 511 && linha[510] != '\n') {
            printf("O arquivo possui uma linha grande demais.\n");
            fclose(arquivo);
            base_liberar(base);
            return 0;
        }

        if (validar_linha(linha, &matricula) == 0) {
            printf("Registro invalido perto da linha %d.\n", base->quantidade + 1);
            fclose(arquivo);
            base_liberar(base);
            return 0;
        }

        tamanho = strlen(linha) + 1;

        if (base->texto_usado + tamanho > capacidade_texto) {
            while (base->texto_usado + tamanho > capacidade_texto) {
                capacidade_texto = capacidade_texto * 2;
            }
            novo_texto = realloc(base->texto, capacidade_texto);
            if (novo_texto == NULL) {
                printf("Faltou memoria para aumentar o texto.\n");
                fclose(arquivo);
                base_liberar(base);
                return 0;
            }
            base->texto = novo_texto;
        }

        base->estudantes[base->quantidade].matricula = matricula;
        base->estudantes[base->quantidade].posicao_texto = base->texto_usado;
        strcpy(base->texto + base->texto_usado, linha);
        base->texto_usado = base->texto_usado + tamanho;
        base->quantidade++;
    }

    if (ferror(arquivo)) {
        printf("Erro durante a leitura do arquivo.\n");
        fclose(arquivo);
        base_liberar(base);
        return 0;
    }

    fclose(arquivo);
    printf("Foram carregados %d estudantes.\n", base->quantidade);
    return 1;
}

char *base_linha(Base *base, Estudante *estudante) {
    if (base == NULL || estudante == NULL) {
        return NULL;
    }
    if (estudante->posicao_texto < 0 ||
        estudante->posicao_texto >= base->texto_usado) {
        return NULL;
    }
    return base->texto + estudante->posicao_texto;
}

void base_liberar(Base *base) {
    if (base == NULL) {
        return;
    }
    free(base->estudantes);
    free(base->texto);
    base->estudantes = NULL;
    base->texto = NULL;
    base->quantidade = 0;
    base->texto_usado = 0;
}
