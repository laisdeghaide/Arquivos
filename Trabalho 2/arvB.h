//Laís Saloum Deghaide, nUSP: 11369767
//Thiago Henrique dos Santos Cardoso, nUSP: 11796594

#ifndef ARVB_H
#define ARVB_H

#include <stdlib.h>
#include <stdio.h>
#define ordem_arvB 5

// Estrutura do cabeçalho
typedef struct {
    char status;
    int noRaiz;
    int RRNproxNo;
} cabecalho_arvB;

// Estrutura dos nós da árvore
typedef struct {
    char folha;
    int nroChavesIndexadas;
    int RRNdoNo;
    int P[ordem_arvB];
    int C[ordem_arvB-1];
    int Pr[ordem_arvB-1];
} no_arvB;

#endif