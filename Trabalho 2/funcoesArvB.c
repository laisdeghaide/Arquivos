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
    no->folha = '1';
    no->nroChavesIndexadas = 0;

    for(int i=0; i<ordem_arvB-1; i++) {
        no->C[i] = -1;
        no->Pr[i] = -1;
        no->P[i] = -1;
    }
    
    no->P[ordem_arvB-1] = -1;
}

// Função que cria nó da árvore
no_arvB *cria_no(FILE *fp_index) {
    cabecalho_arvB *cabecalho = (cabecalho_arvB*)malloc(sizeof(cabecalho_arvB));
    le_cabecalho_arvore(fp_index, cabecalho);

    no_arvB *no = (no_arvB*)malloc(sizeof(no_arvB));
    
    inicializa_no(no);

    fseek(fp_index, (cabecalho->RRNproxNo + 1)*77, SEEK_SET);

    escreve_no_arvore(fp_index, *no);
    cabecalho->RRNproxNo++;

    return no;
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


int busca(FILE *fp_index, int RRN, int chave, int *RRN_encontrado) {
    if(RRN == -1)
        return 0;
    
    no_arvB *no = (no_arvB*)malloc(sizeof(no_arvB));
    le_no_arvore(fp_index, no);

    for(int i=0; i<ordem_arvB-1; i++) {
        if(chave == no->C[i]) {
            *RRN_encontrado = no->Pr[i];
            printf("ERRO. Chave duplicada.");
            break;
        }
    }

    if(*RRN_encontrado != -1) {
        free(no);
        return 1;
    }

    for(int i=0; i<ordem_arvB-1; i++) {
        if(i == ordem_arvB-1) {
            RRN = no->P[ordem_arvB-1];
        }

        else if(chave < no->C[i]) {
            RRN = no->P[i];
            break;
        }
    }

    free(no);
    return busca(fp_index, RRN, chave, RRN_encontrado);
}


void insere_no(FILE *fp_index, int chave, int byteoffset) {
    
    cabecalho_arvB *cabecalho = (cabecalho_arvB*)malloc(sizeof(cabecalho_arvB));
    le_cabecalho_arvore(fp_index, cabecalho);

    no_arvB *no = (no_arvB*)malloc(sizeof(no_arvB));

    // Se ainda nao existe no, cria nó raiz
    if(cabecalho->noRaiz == -1) {
        no = cria_no(fp_index);
        cabecalho->noRaiz = 1;
    }

    // insere normal
    if(no->nroChavesIndexadas < ordem_arvB-1) {
        insere_no(fp_index, );
    }

    // se não, faz split, acha o promote
    //insert(CURRENT_RRN, KEY, PROMO_KEY, PROMO_R_CHILD)

}


void insert(int current_RRN, int RRN, int key, int promo_key, int promo_r_child, FILE *fp_index) {
    if(current_RRN == -1) {
        int promo_key = key;
        int promo_r_child = -1;
        int promo_RRN = RRN;

        return -1;
    }

    no_arvB *no = (no_arvB*)malloc(sizeof(no_arvB));
    le_no_arvore(fp_index, no);


}

