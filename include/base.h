#ifndef BASE_H
#define BASE_H
#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint32_t matricula;
    uint32_t texto_offset; 
} Estudante;

typedef struct {
    Estudante *estudantes;
    size_t quantidade;
    char *texto;
    size_t texto_usado;
} Base;

int base_carregar(Base *base, const char *caminho, size_t limite);
void base_liberar(Base *base);
const char *base_linha(const Base *base, const Estudante *estudante);
#endif
