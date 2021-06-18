//Laís Saloum Deghaide, nUSP: 11369767
//Thiago Henrique dos Santos Cardoso, nUSP: 11796594

#ifndef FUNCOES_ARVB_H
#define FUNCOES_ARVB_H

#include "arvB.h"
#include "funcao-fornecida.h"

void escreve_cabecalho_indice(FILE *fp_index, cabecalho_arvB cabecalho);
void le_cabecalho_indice(FILE *fp_index, cabecalho_arvB *cabecalho);

void escreve_no_arvore(FILE *fp_index, no_arvB no);
void le_no_arvore(FILE *fp_index, no_arvB *no);

void inicializa_arvB(FILE *fp);

void cria_arvB(FILE *fp_bin, FILE *fp_index, int tipo);
void insere_no_veiculo(FILE *fp_bin, int chave, int RRN);

#endif