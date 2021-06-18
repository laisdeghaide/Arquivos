//Laís Saloum Deghaide, nUSP: 11369767
//Thiago Henrique dos Santos Cardoso, nUSP: 11796594

#include "funcoesArvB.h"

void escreve_cabecalho_indice(FILE *fp_index, cabecalho_arvB cabecalho) {
    fwrite(&cabecalho.status, sizeof(char), 1, fp_index);
    fwrite(&cabecalho.noRaiz, sizeof(int), 1, fp_index);
    fwrite(&cabecalho.RRNproxNo, sizeof(int), 1, fp_index);
    
    char lixo[69];
    for(int i=0; i<68; i++) {
        lixo[i] = '@';
    }
    fwrite(lixo, sizeof(char), 68, fp_index);
}

void le_cabecalho_indice(FILE *fp_index, cabecalho_arvB *cabecalho) {
    fread(&cabecalho->status,sizeof(char),1,fp_index);
    fread(&cabecalho->noRaiz,sizeof(int),1,fp_index);
    fread(&cabecalho->RRNproxNo, sizeof(int), 1, fp_index);

    char lixo[69];
    for(int i=0; i<68; i++) {
        lixo[i] = '@';
    }
    fread(lixo, sizeof(char), 68, fp_index);
}

void escreve_no_arvore(FILE *fp_index, no_arvB no) {
    fwrite(&no.folha, sizeof(char), 1, fp_index);
    fwrite(&no.nroChavesIndexadas, sizeof(int), 1, fp_index);
    fwrite(&no.RRNdoNo, sizeof(int), 1, fp_index);

    for(int i=0; i<ordem_arvB-1; i++) {
        fwrite(&no.C[i], sizeof(int), 1, fp_index);
        fwrite(&no.Pr[i], sizeof(int), 1, fp_index);
    }

    for(int i=0; i<ordem_arvB; i++) {
        fwrite(&no.P[i], sizeof(int), 1, fp_index);
    }
}

void le_no_arvore(FILE *fp_index, no_arvB *no) {

    fread(&no->folha, sizeof(char), 1, fp_index);
    fread(&no->nroChavesIndexadas, sizeof(int), 1, fp_index);
    fread(&no->RRNdoNo, sizeof(int), 1, fp_index);

    for(int i=0; i<ordem_arvB-1; i++) {
        fread(&no->C[i], sizeof(int), 1, fp_index);
        fread(&no->Pr[i], sizeof(int), 1, fp_index);
    }

    for(int i=0; i<ordem_arvB; i++) {
        fread(&no->P[i], sizeof(int), 1, fp_index);
    }
}

// Função que cria indice e o inicializa
