//Laís Saloum Deghaide, nUSP: 11369767
//Thiago Henrique dos Santos Cardoso, nUSP: 11796594

#ifndef FUNCOES_LEITURA_H
#define FUNCOES_LEITURA_H

#include "constants.h"

//Funcoes Gerais de Leitura:
int abertura_arquivo(FILE **fp, char *nome, char *modo);
cabecalho_veiculo *le_cabecalho_veiculo(FILE *fp_bin);
cabecalho_linha *le_cabecalho_linha(FILE *fp_bin);
void recebe_dados_veiculo(FILE *fp_bin, dados_veiculo *dados);
void recebe_dados_linha(FILE *fp_bin, dados_linha *dados);

#endif