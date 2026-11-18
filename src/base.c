#include "base.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int validar_linha(char *linha, uint32_t *matricula) {
    size_t tam = strlen(linha);
    while (tam && (linha[tam-1] == '\n' || linha[tam-1] == '\r')) linha[--tam] = '\0';
    if (tam < 12) return 0;
    uint32_t chave = 0;
    for (int i = 0; i < 8; i++) {
        if (linha[i] < '0' || linha[i] > '9') return 0;
        chave = chave * 10u + (uint32_t)(linha[i] - '0');
    }
    if (linha[8] != ';') return 0;
    char *segundo = strchr(linha + 9, ';');
    if (!segundo || segundo == linha + 9 || !segundo[1] || strchr(segundo + 1, ';')) return 0;
    *matricula = chave;
    return 1;
}

int base_carregar(Base *base, const char *caminho, size_t limite) {
    if (!base || !caminho || limite == 0 || limite > UINT32_MAX - 1u) return 0;
    *base = (Base){0};
    FILE *arquivo = fopen(caminho, "rb");
    if (!arquivo) { perror(caminho); return 0; }
    base->estudantes = malloc(limite * sizeof *base->estudantes);
    size_t capacidade_texto = 1u << 20; 
    base->texto = malloc(capacidade_texto);
    if (!base->estudantes || !base->texto) goto falha;

    char linha[512];
    while (base->quantidade < limite) {
        if (fgets(linha, sizeof linha, arquivo) == NULL) break;
        if (strchr(linha, '\n') == NULL && !feof(arquivo)) {
            fprintf(stderr, "Linha muito longa no arquivo de entrada.\n");
            goto falha;
        }
        uint32_t chave;
        if (!validar_linha(linha, &chave)) {
            fprintf(stderr, "Linha %zu invalida: esperado 8 digitos;nome;curso\n", base->quantidade + 1);
            goto falha;
        }
        size_t bytes = strlen(linha) + 1;
        if (base->texto_usado + bytes > UINT32_MAX) {
            fprintf(stderr, "Base de texto excedeu o limite de 4 GiB desta implementacao.\n");
            goto falha;
        }
        if (base->texto_usado + bytes > capacidade_texto) {
            size_t nova = capacidade_texto;
            while (nova < base->texto_usado + bytes) nova *= 2;
            char *tmp = realloc(base->texto, nova);
            if (!tmp) goto falha;
            base->texto = tmp;
            capacidade_texto = nova;
        }
        base->estudantes[base->quantidade++] = (Estudante){chave, (uint32_t)base->texto_usado};
        memcpy(base->texto + base->texto_usado, linha, bytes);
        base->texto_usado += bytes;
    }
    if (ferror(arquivo)) { perror("Erro lendo dados"); goto falha; }
    fclose(arquivo);
    printf("Base carregada: %zu registros, %.1f MiB de texto.\n", base->quantidade,
           (double)base->texto_usado/(1024*1024));
    return 1;
falha:
    fclose(arquivo);
    base_liberar(base);
    return 0;
}

void base_liberar(Base *base) {
    if (!base) return;
    free(base->estudantes);
    free(base->texto);
    *base = (Base){0};
}

const char *base_linha(const Base *base, const Estudante *estudante) {
    if (!base || !estudante || estudante->texto_offset >= base->texto_usado) return NULL;
    return base->texto + estudante->texto_offset;
}
