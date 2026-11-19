#ifndef BASE_H
#define BASE_H

typedef struct {
    int matricula;
    int posicao_texto;
} Estudante;

typedef struct {
    Estudante *estudantes;
    int quantidade;
    char *texto;
    int texto_usado;
} Base;

int base_carregar(Base *base, char *caminho, int limite);
char *base_linha(Base *base, Estudante *estudante);
void base_liberar(Base *base);

#endif
