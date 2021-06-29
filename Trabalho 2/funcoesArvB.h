//Laís Saloum Deghaide, nUSP: 11369767
//Thiago Henrique dos Santos Cardoso, nUSP: 11796594

#ifndef FUNCOES_ARVB_H
#define FUNCOES_ARVB_H

#include "arvB.h"

void escreve_cabecalho_arvore(FILE *fp_index, cabecalho_arvB *cabecalho);
void le_cabecalho_arvore(FILE *fp_index, cabecalho_arvB *cabecalho);

void escreve_no_arvore(FILE *fp_index, no_arvB no);
void le_no_arvore(FILE *fp_index, no_arvB *no);

no_arvB *cria_no(FILE *fp_index, cabecalho_arvB *cabecalho);
void cria_arvB(FILE *fp_bin, FILE *fp_index, int tipo);

int busca(int RRN, int *byteoffset_encontrado, int *RRN_anterior, int chave, FILE *fp_index);
void busca_dados_indice(FILE *fp_bin, FILE *fp_index, int valor, int tipo);

void insere_no(FILE *fp_index, int chave, int byteoffset, cabecalho_arvB *cabecalho_arv);
int insere(int RRN_atual, int RRN, int chave, int *chave_promo, int *filho_promo, FILE *fp_index);

#endif