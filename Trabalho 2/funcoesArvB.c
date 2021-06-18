//Laís Saloum Deghaide, nUSP: 11369767
//Thiago Henrique dos Santos Cardoso, nUSP: 11796594

#include "funcoesArvB.h"
#include "funcoesLeitura.h"

void escreve_cabecalho_arvore(FILE *fp, cabecalho_arvB cabecalho) {
    fwrite(&cabecalho.status, sizeof(char), 1, fp);
    fwrite(&cabecalho.noRaiz, sizeof(int), 1, fp);
    fwrite(&cabecalho.RRNproxNo, sizeof(int), 1, fp);
    
    char lixo[69];
    for(int i=0; i<68; i++) {
        lixo[i] = '@';
    }
    fwrite(lixo, sizeof(char), 68, fp);
}

void le_cabecalho_arvore(FILE *fp, cabecalho_arvB *cabecalho) {
    fread(&cabecalho->status,sizeof(char),1,fp);
    fread(&cabecalho->noRaiz,sizeof(int),1,fp);
    fread(&cabecalho->RRNproxNo, sizeof(int), 1, fp);

    char lixo[69];
    for(int i=0; i<68; i++) {
        lixo[i] = '@';
    }
    fread(lixo, sizeof(char), 68, fp);
}

void escreve_no_arvore(FILE *fp, no_arvB no) {
    fwrite(&no.folha, sizeof(char), 1, fp);
    fwrite(&no.nroChavesIndexadas, sizeof(int), 1, fp);
    fwrite(&no.RRNdoNo, sizeof(int), 1, fp);

    for(int i=0; i<ordem_arvB-1; i++) {
        fwrite(&no.C[i], sizeof(int), 1, fp);
        fwrite(&no.Pr[i], sizeof(int), 1, fp);
    }

    for(int i=0; i<ordem_arvB; i++) {
        fwrite(&no.P[i], sizeof(int), 1, fp);
    }
}

void le_no_arvore(FILE *fp, no_arvB *no) {

    fread(&no->folha, sizeof(char), 1, fp);
    fread(&no->nroChavesIndexadas, sizeof(int), 1, fp);
    fread(&no->RRNdoNo, sizeof(int), 1, fp);

    for(int i=0; i<ordem_arvB-1; i++) {
        fread(&no->C[i], sizeof(int), 1, fp);
        fread(&no->Pr[i], sizeof(int), 1, fp);
    }

    for(int i=0; i<ordem_arvB; i++) {
        fread(&no->P[i], sizeof(int), 1, fp);
    }
}

// Função que inicializa a árvore
void inicializa_arvB(FILE *fp) {
    cabecalho_arvB cabecalho;

    cabecalho.status = 1;
    cabecalho.noRaiz = -1;
    cabecalho.RRNproxNo = 0;

    escreve_cabecalho_arvore(fp, cabecalho);
}

// Função que cria indice para arvore de veiculo
void cria_arvB(FILE *fp_bin, FILE *fp_index, int tipo) {
    no_arvB *dados = (no_arvB *)malloc(sizeof(no_arvB));

    // Inicializando 
    inicializa_arvB(fp_index);

    int RRN = 0;
    
    char buffer[70];
    while(fgets(buffer, sizeof(buffer), fp_bin) != NULL) {
        if(tipo == 9) {
            dados_veiculo *dados = (dados_veiculo*) malloc(sizeof(dados_veiculo));
            recebe_dados_veiculo(fp_bin, dados); 
        }

    }
}

void insere_no_veiculo(FILE *fp_index, int chave, int RRN) {

}