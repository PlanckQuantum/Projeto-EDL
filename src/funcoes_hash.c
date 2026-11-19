#include "funcoes_hash.h"

int calcular_hash(int matricula, int capacidade, int tipo) {
    int resultado;
    int divisor;
    int digito;
    int i;

    if (capacidade <= 0) {
        return 0;
    }

    if (tipo == HASH_DIVISAO) {
        return matricula % capacidade;
    }

    resultado = 0;
    divisor = 10000000;
    for (i = 0; i < 8; i++) {
        digito = (matricula / divisor) % 10;
        resultado = (resultado * 31 + digito) % capacidade;
        divisor = divisor / 10;
    }
    return resultado;
}

int eh_primo(int numero) {
    int i;
    if (numero < 2) {
        return 0;
    }
    for (i = 2; i * i <= numero; i++) {
        if (numero % i == 0) {
            return 0;
        }
    }
    return 1;
}

int proximo_primo(int numero) {
    while (eh_primo(numero) == 0) {
        numero++;
    }
    return numero;
}
