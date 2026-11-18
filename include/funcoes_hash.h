#ifndef FUNCOES_HASH_H
#define FUNCOES_HASH_H
#include <stdint.h>
#include <stddef.h>

typedef enum { HASH_DIVISAO = 0, HASH_FNV1A = 1 } TipoHash;
size_t calcular_hash(uint32_t matricula, size_t capacidade, TipoHash tipo);
const char *nome_funcao_hash(TipoHash tipo);
size_t proximo_primo(size_t valor);
#endif
