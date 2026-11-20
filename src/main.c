#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "base.h"
#include "lista.h"
#include "hash_aberta.h"
#include "hash_encadeada.h"
#include "funcoes_hash.h"

#define TOTAL_BUSCAS 10000

double tempo_agora(void) {
    return (double)clock() / CLOCKS_PER_SEC;
}

int testar_lista(FILE *saida, Lista *lista, Base *base,
                 int anterior, int limite, int consultas[],
                 int volume, int percentual) {
    int i;
    double inicio;
    double tempo_insercao;
    double tempo_busca;
    Estudante *encontrado;

    inicio = tempo_agora();
    for (i = anterior; i < limite; i++) {
        if (lista_inserir(lista, &base->estudantes[i]) == 0) {
            printf("Erro ao inserir na lista.\n");
            return 0;
        }
    }
    tempo_insercao = tempo_agora() - inicio;

    inicio = tempo_agora();
    for (i = 0; i < TOTAL_BUSCAS; i++) {
        encontrado = lista_buscar(lista, consultas[i]);
        if (encontrado == NULL || encontrado->matricula != consultas[i]) {
            printf("Erro ao buscar na lista.\n");
            return 0;
        }
    }
    tempo_busca = tempo_agora() - inicio;

    fprintf(saida, "%d,%d,%d,lista,nenhuma,%.6f,%.6f,,,\n",
            volume, percentual, limite, tempo_insercao, tempo_busca);
    printf("    Lista: OK\n");
    return 1;
}

int testar_aberta(FILE *saida, HashAberta *hash, Base *base,
                  int anterior, int limite, int consultas[],
                  int volume, int percentual, char *nome_hash) {
    int i;
    int colisoes_antes;
    double inicio;
    double tempo_insercao;
    double tempo_busca;
    double fator_carga;
    Estudante *encontrado;

    colisoes_antes = hash->colisoes;
    inicio = tempo_agora();
    for (i = anterior; i < limite; i++) {
        if (aberta_inserir(hash, &base->estudantes[i]) == 0) {
            printf("Erro ao inserir na hash aberta (%s).\n", nome_hash);
            return 0;
        }
    }
    tempo_insercao = tempo_agora() - inicio;

    inicio = tempo_agora();
    for (i = 0; i < TOTAL_BUSCAS; i++) {
        encontrado = aberta_buscar(hash, consultas[i]);
        if (encontrado == NULL || encontrado->matricula != consultas[i]) {
            printf("Erro ao buscar na hash aberta (%s).\n", nome_hash);
            return 0;
        }
    }
    tempo_busca = tempo_agora() - inicio;

    fator_carga = (double)hash->quantidade / hash->capacidade;
    fprintf(saida, "%d,%d,%d,hash_aberta,%s,%.6f,%.6f,%d,%.6f,%d\n",
            volume, percentual, limite, nome_hash,
            tempo_insercao, tempo_busca, hash->colisoes - colisoes_antes,
            fator_carga, hash->capacidade);
    printf("    Hash aberta (%s): OK\n", nome_hash);
    return 1;
}

int testar_encadeada(FILE *saida, HashEncadeada *hash, Base *base,
                     int anterior, int limite, int consultas[],
                     int volume, int percentual, char *nome_hash) {
    int i;
    int colisoes_antes;
    double inicio;
    double tempo_insercao;
    double tempo_busca;
    double fator_carga;
    Estudante *encontrado;

    colisoes_antes = hash->colisoes;
    inicio = tempo_agora();
    for (i = anterior; i < limite; i++) {
        if (encadeada_inserir(hash, &base->estudantes[i]) == 0) {
            printf("Erro ao inserir na hash encadeada (%s).\n", nome_hash);
            return 0;
        }
    }
    tempo_insercao = tempo_agora() - inicio;

    inicio = tempo_agora();
    for (i = 0; i < TOTAL_BUSCAS; i++) {
        encontrado = encadeada_buscar(hash, consultas[i]);
        if (encontrado == NULL || encontrado->matricula != consultas[i]) {
            printf("Erro ao buscar na hash encadeada (%s).\n", nome_hash);
            return 0;
        }
    }
    tempo_busca = tempo_agora() - inicio;

    fator_carga = (double)hash->quantidade / hash->capacidade;
    fprintf(saida, "%d,%d,%d,hash_encadeada,%s,%.6f,%.6f,%d,%.6f,%d\n",
            volume, percentual, limite, nome_hash,
            tempo_insercao, tempo_busca, hash->colisoes - colisoes_antes,
            fator_carga, hash->capacidade);
    printf("    Hash encadeada (%s): OK\n", nome_hash);
    return 1;
}

int executar_experimento(FILE *saida, Base *base, int volume) {
    int percentuais[3] = {30, 60, 100};
    int consultas[TOTAL_BUSCAS];
    int capacidade;
    int anterior = 0;
    int fase;
    int limite;
    int i;
    int ok = 1;

    Lista *lista;
    HashAberta *aberta_divisao;
    HashAberta *aberta_polinomial;
    HashEncadeada *encadeada_divisao;
    HashEncadeada *encadeada_polinomial;

    capacidade = proximo_primo(volume * 10 / 7 + 1);
    printf("\nVOLUME %d - tamanho das tabelas: %d\n", volume, capacidade);

    lista = lista_criar();
    aberta_divisao = aberta_criar(capacidade, HASH_DIVISAO);
    aberta_polinomial = aberta_criar(capacidade, HASH_POLINOMIAL);
    encadeada_divisao = encadeada_criar(capacidade, HASH_DIVISAO);
    encadeada_polinomial = encadeada_criar(capacidade, HASH_POLINOMIAL);

    if (lista == NULL || aberta_divisao == NULL ||
        aberta_polinomial == NULL || encadeada_divisao == NULL ||
        encadeada_polinomial == NULL) {
        printf("Faltou memoria para criar as estruturas.\n");
        ok = 0;
    }

    for (fase = 0; fase < 3 && ok == 1; fase++) {
        limite = volume * percentuais[fase] / 100;
        if (limite == anterior) {
            printf("Volume muito pequeno para dividir em tres etapas.\n");
            ok = 0;
            break;
        }

        srand(2026 + volume + fase);
        for (i = 0; i < TOTAL_BUSCAS; i++) {
            int indice = rand() % limite;
            consultas[i] = base->estudantes[indice].matricula;
        }

        printf("  ETAPA %d%% (%d estudantes)\n", percentuais[fase], limite);
        fflush(stdout);

        if (testar_lista(saida, lista, base, anterior, limite,
                         consultas, volume, percentuais[fase]) == 0) {
            ok = 0;
            break;
        }
        if (testar_aberta(saida, aberta_divisao, base, anterior, limite,
                          consultas, volume, percentuais[fase], "divisao") == 0) {
            ok = 0;
            break;
        }
        if (testar_aberta(saida, aberta_polinomial, base, anterior, limite,
                          consultas, volume, percentuais[fase], "polinomial") == 0) {
            ok = 0;
            break;
        }
        if (testar_encadeada(saida, encadeada_divisao, base, anterior, limite,
                             consultas, volume, percentuais[fase], "divisao") == 0) {
            ok = 0;
            break;
        }
        if (testar_encadeada(saida, encadeada_polinomial, base, anterior, limite,
                             consultas, volume, percentuais[fase], "polinomial") == 0) {
            ok = 0;
            break;
        }

        anterior = limite;
        fflush(saida);
    }

    lista_liberar(lista);
    aberta_liberar(aberta_divisao);
    aberta_liberar(aberta_polinomial);
    encadeada_liberar(encadeada_divisao);
    encadeada_liberar(encadeada_polinomial);
    return ok;
}

int main(void) {
    int volumes[3] = {100000, 1000000, 10000000};
    int numero_volumes = 3;
    int maior_volume = 10000000;
    int teste_pequeno = 0;
    int i;
    int ok = 1;
    char *caminho = "dados/estudantes.txt";
    Base base;
    FILE *saida;

    if (teste_pequeno == 1) {
        maior_volume = 30;
        volumes[0] = 30;
        numero_volumes = 1;
    }

    if (base_carregar(&base, caminho, maior_volume) == 0) {
        return 1;
    }
    if (base.quantidade < maior_volume) {
        printf("A base tem somente %d registros; faltam dados.\n", base.quantidade);
        base_liberar(&base);
        return 1;
    }

    saida = fopen("resultados.csv", "w");
    if (saida == NULL) {
        printf("Nao foi possivel criar resultados.csv.\n");
        base_liberar(&base);
        return 1;
    }
    fprintf(saida, "volume,percentual,registros,estrutura,funcao_hash,tempo_insercao_s,tempo_busca_s,colisoes_intervalo,fator_carga,capacidade\n");

    for (i = 0; i < numero_volumes; i++) {
        if (executar_experimento(saida, &base, volumes[i]) == 0) {
            ok = 0;
            break;
        }
    }

    fclose(saida);
    base_liberar(&base);

    if (ok == 1) {
        printf("\nConcluido! Resultados salvos em resultados.csv\n");
        return 0;
    }
    printf("\nO experimento foi interrompido por um erro.\n");
    return 1;
}
