//Laís Saloum Deghaide, nUSP: 11369767
//Thiago Henrique dos Santos Cardoso, nUSP: 11796594

#include "funcoesArvB.h"
#include "funcoesLeitura.h"

void escreve_cabecalho_arvore(FILE *fp, cabecalho_arvB cabecalho) {
    fseek(fp, 0, SEEK_SET);
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
    fseek(fp, 0, SEEK_SET);
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


// Inicializa nó
void inicializa_no(no_arvB *no) {
no->folha = -1;
    no->nroChavesIndexadas = 0;

    for(int i=0; i<ordem_arvB-1; i++) {
        no->C[i] = -1;
        no->Pr[i] = -1;
        no->P[i] = -1;
    }
    
    no->P[ordem_arvB-1] = -1;
}

// Função que cria nó da árvore
void cria_no(FILE *fp_index) {
    cabecalho_arvB *cabecalho = (cabecalho_arvB*)malloc(sizeof(cabecalho_arvB));
    le_cabecalho_arvore(fp_index, cabecalho);

    no_arvB *no = (no_arvB*)malloc(sizeof(no_arvB));
    
    inicializa_no(no);

    fseek(fp_index, (cabecalho->RRNproxNo + 1)*77, SEEK_SET);

    escreve_no_arvore(fp_index, *no);
    cabecalho->RRNproxNo++;
}

// Função que inicializa a árvore
void inicializa_arvB(FILE *fp) {
    cabecalho_arvB cabecalho;

    cabecalho.status = 1;
    cabecalho.noRaiz = -1;
    cabecalho.RRNproxNo = 0;

    escreve_cabecalho_arvore(fp, cabecalho);
}

// Função que cria indice para arvore
void cria_arvB(FILE *fp_bin, FILE *fp_index, int tipo) {

    // Inicializando a arvore
    inicializa_arvB(fp_index);

    // Se for veiculo, le a quantidade de registros e os insere na árvore
    if(tipo == 9) {

        // Aloca espaço para o cabecalho, lê ele e pega a quantidade de registros que tem no arquivo binário
        cabecalho_veiculo *cabecalho = (cabecalho_veiculo*)malloc(sizeof(cabecalho_veiculo));
        le_cabecalho_veiculo(fp_bin, cabecalho);
        int qtd_registros = cabecalho->nroRegistros;

        dados_veiculo *dados = (dados_veiculo*) malloc(sizeof(dados_veiculo));
        
        // le cada um dos registros de veiculo e insere na arvore
        for(int i=0; i<qtd_registros; i++) {
            
            int offset = ftell(fp_bin);
            recebe_dados_veiculo(fp_bin, dados); 

            int chave = convertePrefixo(dados->prefixo);
            insere_no(fp_index, chave, offset);
        }

        free(cabecalho);
        free(dados);
    }

    else if(tipo == 10) {
        
        // Aloca espaço para o cabecalho, lê ele e pega a quantidade de registros que tem no arquivo binário
        cabecalho_linha *cabecalho = (cabecalho_linha*)malloc(sizeof(cabecalho_linha));
        le_cabecalho_linha(fp_bin, cabecalho);
        int qtd_registros = cabecalho->nroRegistros;

        dados_linha *dados = (dados_linha*) malloc(sizeof(dados_linha));
        
        // le cada um dos registros de veiculo e insere na arvore
        for(int i=0; i<qtd_registros; i++) {
            
            int offset = ftell(fp_bin);
            recebe_dados_linha(fp_bin, dados); 
            insere_no(fp_index, dados->codLinha, offset);
        }

        free(cabecalho);
        free(dados);
    }
}

void insere_no(FILE *fp_index, int chave, int byteoffset) {

}