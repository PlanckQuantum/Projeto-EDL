#include "base.h"
#include "lista.h"
#include "hash_aberta.h"
#include "hash_encadeada.h"
#include "funcoes_hash.h"
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define QUANTIDADE_ESTRUTURAS 5

typedef struct {
    size_t volumes[16];
    size_t numero_volumes;
    size_t buscas;
    uint64_t semente;
    const char *arquivo;
    const char *saida;
} Config;

static void ajuda(const char *programa) {
    printf("Uso: %s --input dados/estudantes.txt [opcoes]\n", programa);
    printf("  --volumes 100000,1000000,10000000 (padrao)\n");
    printf("  --searches 10000 (padrao, obrigatorio nos testes oficiais)\n");
    printf("  --seed 2026 (padrao)\n");
    printf("  --output resultados/experimentos.csv (padrao)\n");
    printf("Exemplo pequeno: %s --input dados/amostra.txt --volumes 29 --searches 10000\n", programa);
}
static int inteiro_positivo(const char *texto, size_t *valor) {
    if (!texto || !*texto || *texto == '-') return 0;
    errno = 0; char *fim;
    unsigned long long v = strtoull(texto, &fim, 10);
    if (errno || *fim || !v || v > SIZE_MAX) return 0;
    *valor = (size_t)v;
    return 1;
}
static int volumes_ler(Config *c, const char *texto) {
    char *copiar = malloc(strlen(texto) + 1);
    if (copiar) strcpy(copiar, texto);
    if (!copiar) return 0;
    c->numero_volumes = 0;
    for (char *token = strtok(copiar, ","); token; token = strtok(NULL, ",")) {
        if (c->numero_volumes == 16 || !inteiro_positivo(token, &c->volumes[c->numero_volumes])) {
            free(copiar); return 0;
        }
        if (c->numero_volumes && c->volumes[c->numero_volumes] <= c->volumes[c->numero_volumes-1]) {
            free(copiar); return 0;
        }
        ++c->numero_volumes;
    }
    free(copiar);
    return c->numero_volumes > 0;
}
static int args_ler(Config *c, int argc, char **argv) {
    *c = (Config){.volumes={100000,1000000,10000000}, .numero_volumes=3,
                  .buscas=10000, .semente=2026, .saida="resultados/experimentos.csv"};
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "--help")) { ajuda(argv[0]); exit(0); }
        if (i+1 == argc) return 0;
        const char *valor = argv[++i];
        if (!strcmp(argv[i-1], "--input")) c->arquivo = valor;
        else if (!strcmp(argv[i-1], "--output")) c->saida = valor;
        else if (!strcmp(argv[i-1], "--volumes")) { if (!volumes_ler(c, valor)) return 0; }
        else if (!strcmp(argv[i-1], "--searches")) { if (!inteiro_positivo(valor, &c->buscas)) return 0; }
        else if (!strcmp(argv[i-1], "--seed")) {
            size_t s; if (!inteiro_positivo(valor, &s)) return 0; c->semente = s;
        } else return 0;
    }
    return c->arquivo != NULL;
}
static double relogio(void) {
    struct timespec t;
    if (timespec_get(&t, TIME_UTC) != TIME_UTC) { fprintf(stderr, "Erro no relogio.\n"); exit(1); }
    return (double)t.tv_sec + (double)t.tv_nsec / 1000000000.0;
}
static uint64_t randomizar(uint64_t *estado) {
    uint64_t x = *estado;
    x ^= x >> 12; x ^= x << 25; x ^= x >> 27;
    *estado = x;
    return x * UINT64_C(2685821657736338717);
}

static size_t capacidade_tabelas(size_t volume) {
    size_t pedido = (volume / 7) * 10 + ((volume % 7) * 10 + 6) / 7;
    return proximo_primo(pedido);
}

typedef struct {
    const char *nome;
    const char *funcao;
    Lista *lista;
    HashAberta *aberta;
    HashEncadeada *encadeada;
} Estrutura;

static int inicializar(Estrutura e[QUANTIDADE_ESTRUTURAS], const Base *base, size_t capacidade) {
    memset(e, 0, QUANTIDADE_ESTRUTURAS * sizeof *e);
    e[0].nome = "lista"; e[0].funcao = "nenhuma";
    e[1].nome = "sondagem_linear"; e[1].funcao = "divisao";
    e[2].nome = "sondagem_linear"; e[2].funcao = "fnv1a";
    e[3].nome = "encadeamento_separado"; e[3].funcao = "divisao";
    e[4].nome = "encadeamento_separado"; e[4].funcao = "fnv1a";
    e[0].lista = lista_criar();
    e[1].aberta = aberta_criar(base, capacidade, HASH_DIVISAO);
    e[2].aberta = aberta_criar(base, capacidade, HASH_FNV1A);
    e[3].encadeada = encadeada_criar(capacidade, HASH_DIVISAO);
    e[4].encadeada = encadeada_criar(capacidade, HASH_FNV1A);
    return e[0].lista && e[1].aberta && e[2].aberta && e[3].encadeada && e[4].encadeada;
}
static void liberar(Estrutura e[QUANTIDADE_ESTRUTURAS]) {
    for (int j=0; j<QUANTIDADE_ESTRUTURAS; ++j) {
        lista_liberar(e[j].lista);
        aberta_liberar(e[j].aberta);
        encadeada_liberar(e[j].encadeada);
    }
}
static int inserir(Estrutura *e, const Base *base, size_t indice) {
    const Estudante *estudante = &base->estudantes[indice];
    if (e->lista) return lista_inserir(e->lista, estudante);
    if (e->aberta) return aberta_inserir(e->aberta, indice);
    return encadeada_inserir(e->encadeada, estudante);
}
static const Estudante *buscar(const Estrutura *e, uint32_t chave) {
    if (e->lista) return lista_buscar(e->lista, chave);
    if (e->aberta) return aberta_buscar(e->aberta, chave);
    return encadeada_buscar(e->encadeada, chave);
}
static uint64_t colisoes(const Estrutura *e) {
    if (e->aberta) return e->aberta->colisoes;
    if (e->encadeada) return e->encadeada->colisoes;
    return 0;
}
static size_t quantidade(const Estrutura *e) {
    if (e->lista) return e->lista->quantidade;
    if (e->aberta) return e->aberta->quantidade;
    return e->encadeada->quantidade;
}

static int experimento(FILE *saida, const Base *base, size_t volume, size_t buscas,
                       uint64_t semente) {
    const int percentuais[] = {30, 60, 100};
    size_t limites[] = {volume*3/10, volume*6/10, volume};
    size_t capacidade = capacidade_tabelas(volume);
    if (!limites[0] || limites[1] == limites[0] || capacidade >= UINT32_MAX) {
        fprintf(stderr, "Volume invalido ou fora do limite.\n"); return 0;
    }
    printf("\nVOLUME: %zu | capacidade fixa das quatro tabelas: %zu | alpha max: %.4f\n",
           volume, capacidade, (double)volume / capacidade);
    fflush(stdout);
    Estrutura e[QUANTIDADE_ESTRUTURAS];
    if (!inicializar(e, base, capacidade)) { liberar(e); return 0; }
    size_t anterior = 0;
    int sucesso = 1;
    for (int fase=0; fase<3 && sucesso; ++fase) {
        size_t limite = limites[fase];
        uint32_t *consultas = malloc(buscas * sizeof *consultas);
        if (!consultas) { sucesso = 0; break; }
        uint64_t estado = semente ^ (uint64_t)volume ^ ((uint64_t)fase << 32);
        if (!estado) estado = 1;
        for (size_t i=0; i<buscas; ++i) {
            size_t indice = (size_t)(randomizar(&estado) % limite);
            consultas[i] = base->estudantes[indice].matricula;
        }
        printf("  Etapa %d%% (%zu registros): ", percentuais[fase], limite);
        fflush(stdout);
        for (int j=0; j<QUANTIDADE_ESTRUTURAS; ++j) {
            uint64_t antes = colisoes(&e[j]);
            double inicio = relogio();
            for (size_t i=anterior; i<limite; ++i) {
                if (!inserir(&e[j], base, i)) {
                    fprintf(stderr, "\nFalha/duplicata na insercao: estrutura=%s hash=%s linha=%zu\n",
                            e[j].nome, e[j].funcao, i+1);
                    sucesso = 0;
                    break;
                }
            }
            double tempo_insercao = relogio() - inicio;
            if (!sucesso) break;
            if (quantidade(&e[j]) != limite) { sucesso = 0; break; }
            uint64_t soma = 0;
            inicio = relogio();
            for (size_t k=0; k<buscas; ++k) {
                const Estudante *achado = buscar(&e[j], consultas[k]);
                if (!achado || achado->matricula != consultas[k]) {
                    fprintf(stderr, "\nBusca incorreta: %s (%s), chave %" PRIu32 "\n",
                            e[j].nome, e[j].funcao, consultas[k]);
                    sucesso = 0; break;
                }
                soma += achado->matricula;
            }
            double tempo_busca = relogio() - inicio;
            if (!sucesso) break;
            if (e[j].lista) {
                fprintf(saida, "%zu,%d,%zu,%s,%s,%.9f,%.9f,,,%zu,%" PRIu64 "\n",
                        volume, percentuais[fase], limite, e[j].nome, e[j].funcao,
                        tempo_insercao, tempo_busca, (size_t)0, soma);
            } else {
                fprintf(saida, "%zu,%d,%zu,%s,%s,%.9f,%.9f,%" PRIu64 ",%.9f,%zu,%" PRIu64 "\n",
                        volume, percentuais[fase], limite, e[j].nome, e[j].funcao,
                        tempo_insercao, tempo_busca, colisoes(&e[j])-antes,
                        (double)limite/(double)capacidade, capacidade, soma);
            }
            printf("%s/%s OK  ", e[j].nome, e[j].funcao);
            fflush(stdout);
        }
        free(consultas);
        anterior = limite;
        printf("\n");
        fflush(saida);
    }
    liberar(e);
    return sucesso;
}

int main(int argc, char **argv) {
    Config config;
    if (!args_ler(&config, argc, argv)) { ajuda(argv[0]); return 1; }
    if (config.buscas != 10000) fprintf(stderr, "AVISO: use --searches 10000 nos experimentos oficiais.\n");
    Base base;
    size_t maximo = config.volumes[config.numero_volumes-1];
    if (!base_carregar(&base, config.arquivo, maximo)) return 1;
    if (base.quantidade < maximo) {
        fprintf(stderr, "Base possui apenas %zu registros; o maior volume pede %zu.\n",
                base.quantidade, maximo);
        base_liberar(&base); return 1;
    }
    FILE *saida = fopen(config.saida, "w");
    if (!saida) { perror(config.saida); base_liberar(&base); return 1; }
    fprintf(saida, "volume,percentual,registros,estrutura,funcao_hash,tempo_insercao_s,tempo_busca_s,colisoes_intervalo,fator_carga,capacidade,checksum\n");
    int sucesso = 1;
    for (size_t i=0; i<config.numero_volumes; ++i) {
        if (!experimento(saida, &base, config.volumes[i], config.buscas, config.semente)) {
            sucesso = 0; break;
        }
    }
    fclose(saida);
    base_liberar(&base);
    if (sucesso) printf("\nExperimentos concluidos. CSV: %s\n", config.saida);
    else fprintf(stderr, "Experimentos interrompidos: verifique a mensagem acima.\n");
    return sucesso ? 0 : 1;
}
