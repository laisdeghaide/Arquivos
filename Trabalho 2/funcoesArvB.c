//Laís Saloum Deghaide, nUSP: 11369767
//Thiago Henrique dos Santos Cardoso, nUSP: 11796594

#include "funcoesArvB.h"

void escreve_cabecalho_indice(FILE *fp_index, cabecalho_arvB cabecalho) {
    fwrite(&cabecalho.status, sizeof(char), 1, fp_index);
    fwrite(&cabecalho.noRaiz, sizeof(int), 1, fp_index);
    fwrite(&cabecalho.RRNproxNo, sizeof(int), 1, fp_index);
    
    for(int i=0; i<68; i++) {
        fwrite('@', sizeof(char), 1, fp_index);
    }
}

void le_cabecalho_indice(FILE *fp_index, cabecalho_arvB *cabecalho) {
    fread(&cabecalho->status,sizeof(char),1,fp_index);
    fread(&cabecalho->noRaiz,sizeof(int),1,fp_index);
    fread(&cabecalho->RRNproxNo, sizeof(int), 1, fp_index);

    for(int i=0; i<68; i++) {
        fread('@', sizeof(char), 1, fp_index);
    }
}

void escreve_no_arvore(FILE *fp_index, no_arvB no) {
    fwrite(&no.folha, sizeof(char), 1, fp_index);
    fwrite(&no.nroChavesIndexadas, sizeof(int), 1, fp_index);
    fwrite(&no.RRNdoNo, sizeof(int), 1, fp_index);

    for(int i=0; i<ordem_arvB-1; i++) {
        fwrite(&no.P[i], sizeof(int), 1, fp_index);
        fwrite(&no.C[i], sizeof(int), 1, fp_index);
        fwrite(&no.Pr[i], sizeof(int), 1, fp_index);
    }

    fwrite(&no.P[ordem_arvB-1], sizeof(int), 1, fp_index);
}

void le_no_arvore(FILE *fp_index, no_arvB *no) {
    fread(&no->folha, sizeof(char), 1, fp_index);
    fread(&no->nroChavesIndexadas, sizeof(int), 1, fp_index);
    fread(&no->RRNdoNo, sizeof(int), 1, fp_index);

    for(int i=0; i<ordem_arvB-1; i++) {
        fread(&no->P[i], sizeof(int), 1, fp_index);
        fread(&no->C[i], sizeof(int), 1, fp_index);
        fread(&no->Pr[i], sizeof(int), 1, fp_index);
    }

    fread(&no->P[ordem_arvB-1], sizeof(int), 1, fp_index);
}