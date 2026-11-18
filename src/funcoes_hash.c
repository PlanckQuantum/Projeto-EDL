#include "funcoes_hash.h"

size_t calcular_hash(uint32_t matricula, size_t capacidade, TipoHash tipo) {
    if (!capacidade) return 0;
    if (tipo == HASH_DIVISAO) return (size_t)matricula % capacidade;
    uint32_t hash = 2166136261u;
    uint32_t divisor = 10000000u;
    for (int i = 0; i < 8; ++i) {
        unsigned char c = (unsigned char)('0' + (matricula / divisor) % 10u);
        hash ^= (uint32_t)c;
        hash *= 16777619u;
        divisor /= 10u;
    }
    return (size_t)hash % capacidade;
}
const char *nome_funcao_hash(TipoHash tipo) {
    return tipo == HASH_DIVISAO ? "divisao" : "fnv1a";
}
static int eh_primo(size_t n) {
    if (n < 2) return 0;
    if (n % 2 == 0) return n == 2;
    for (size_t i = 3; i <= n / i; i += 2) if (n % i == 0) return 0;
    return 1;
}
size_t proximo_primo(size_t n) {
    if (n <= 2) return 2;
    if (n % 2 == 0) ++n;
    while (!eh_primo(n)) n += 2;
    return n;
}
