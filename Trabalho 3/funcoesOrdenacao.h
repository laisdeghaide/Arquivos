//Laís Saloum Deghaide, nUSP: 11369767
//Thiago Henrique dos Santos Cardoso, nUSP: 11796594

#ifndef FUNCOES_ORDENACAO_H
#define FUNCOES_ORDENACAO_H

#include "constants.h"
#include "funcoesLeitura.h"
#include "funcoesEscrita.h"
#include "funcoesArvB.h"
#include "arvB.h"

void ordena_arquivo(FILE *fp_ordenado, FILE *fp_desordenado, int tipo, cabecalho_linha *cabecalho_l, cabecalho_veiculo *cabecalho_v);

#endif
