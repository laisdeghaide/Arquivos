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

no_arvB *cria_no(FILE *fp_index);
void inicializa_no(no_arvB *no);

void inicializa_arvB(FILE *fp);
void cria_arvB(FILE *fp_bin, FILE *fp_index, int tipo);

int busca(FILE *fp_index, int RRN, int chave, int *RRN_encontrado);

void insere_no(FILE *fp_index, int chave, int byteoffset);
void insert(int current_RRN, int RRN, int key, int promo_key, int promo_r_child, FILE *fp_index);

#endif