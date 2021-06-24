//Laís Saloum Deghaide, nUSP: 11369767
//Thiago Henrique dos Santos Cardoso, nUSP: 11796594

#include "funcoesArvB.h"
#include "funcoesLeitura.h"
#include "funcoesEscrita.h"

// Função que nos auxilia na escrita de cada um dos campos do cabecalho da arvore, alocando lixo no fim
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

// Função responsável pela leitura dos campos do cabecalho da arvore
void le_cabecalho_arvore(FILE *fp, cabecalho_arvB *cabecalho) {
    fseek(fp, 0, SEEK_SET);
    fread(&cabecalho->status,sizeof(char),1,fp);
    fread(&cabecalho->noRaiz,sizeof(int),1,fp);
    fread(&cabecalho->RRNproxNo, sizeof(int), 1, fp);

    char lixo[69];
    fread(lixo, sizeof(char), 68, fp);
}

// Função responsável pela escrita das paginas de disco
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

// Função responsável por ler página de disco
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

// Função que inicializa pagina de disco
// Inicialmente é marcado como folha, e todos os outros campos como nulos (-1)
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
// Responsável por preencher as paginas de disco 
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
// Inicialmente não tem nó raiz e nem proxNo
void inicializa_arvB(FILE *fp) {
    cabecalho_arvB cabecalho;

    cabecalho.status = 1;
    cabecalho.noRaiz = -1;
    cabecalho.RRNproxNo = 0;

    escreve_cabecalho_arvore(fp, cabecalho);
}

// Função que cria indice para árvore
// Recebe como parametro:
// os ponteiros fp_bin (ponteiro do arquivo de registros), fp_index (ponteiro da árvore)
// e tipo (se linha ou veiculo)
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

            // Lê o byteoffset do registro 
            int offset = ftell(fp_bin);

            // Lê os dados do registro
            recebe_dados_veiculo(fp_bin, dados); 

            // Converte a chave para int
            int chave = convertePrefixo(dados->prefixo);

            //Se o registro não estiver marcado como removido, inserimos
            if(dados->removido != '0') 
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
            
            // Lê o byteoffset do registro
            int offset = ftell(fp_bin);

            // Lê os dados do registro
            recebe_dados_linha(fp_bin, dados); 

            // Se o registro não estiver marcado como removido, inserimos
            if(dados->removido != '0') 
                insere_no(fp_index, dados->codLinha, offset);
        }

        free(cabecalho);
        free(dados);
    }
}


int busca(int RRN, int *byteoffset_encontrado, int chave, FILE *fp_index) {
    // Chave de busca não encontrada
    if(RRN == -1)
        return 0;
    
    // posiciona o ponteiro na pagina de disco desejada
    fseek(fp_index, (RRN+1)*77, SEEK_SET);

    // lê a pagina de disco
    no_arvB *no = (no_arvB*)malloc(sizeof(no_arvB));
    le_no_arvore(fp_index, no);

    // pesquisa a página, procurando a chave de busca
    for(int i=0; i<ordem_arvB-1; i++) {
        if(chave == no->C[i]) {
            *byteoffset_encontrado = no->Pr[i];
            break;
        }
    }

    // Se a chave existe, retorna 1 (== found)
    if(*byteoffset_encontrado != -1) {
        free(no);
        return 1;
    }

    // Se não, acha o próximo RRN para continuar a pesquisa
    for(int i=0; i<ordem_arvB-1; i++) {

        // Se chegou no penúltimo RRN, significa que até aqui não encontrou, 
        // então o RRN vai ser o último RRN presente na árvore
        if(i == ordem_arvB-1) {
            RRN = no->P[ordem_arvB-1];
        }

        // Se não, o RRN vai ser o próximo
        else if(chave < no->C[i]) {
            RRN = no->P[i];
            break;
        }
    }

    free(no);
    return busca(RRN, byteoffset_encontrado, chave, fp_index);
}


void busca_dados_indice(FILE *fp_bin, FILE *fp_index, int valor, int tipo) {
    cabecalho_arvB *cabecalho = (cabecalho_arvB*)malloc(sizeof(cabecalho_arvB));
    le_cabecalho_arvore(fp_index, cabecalho);

    int byteoffset_encontrado = busca(cabecalho->noRaiz, 0, valor, fp_index);

    // Se o registro existe
    if(byteoffset_encontrado != 0) {

        if(tipo == 11) {
            fseek(fp_bin, 175*(byteoffset_encontrado+1), SEEK_SET);
            dados_veiculo *dados = (dados_veiculo*)malloc(sizeof(dados_veiculo));
            cabecalho_veiculo *cabecalho_veiculo = (cabecalho_veiculo*)malloc(sizeof(cabecalho_veiculo));
            recebe_dados_veiculos(fp_bin, dados);
            
            printa_veiculo(dados, cabecalho_veiculo);
        }

        else if(tipo == 12) {
            fseek(fp_bin, 83*(byteoffset_encontrado+1), SEEK_SET); // CHECAR TAMANHO DO CABECALHO DE LINHA
            dados_linha *dados = (dados_veiculo*)malloc(sizeof(dados_linha));
            cabecalho_linha *cabecalho_linha = (cabecalho_linha*)malloc(sizeof(cabecalho));
            recebe_dados_linha(fp_bin, dados);
            printa_linha(dados, cabecalho);
        }
    }
    else {
         printf("Registro inexistente.\n");
    }
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

