//Laís Saloum Deghaide, nUSP: 11369767
//Thiago Henrique dos Santos Cardoso, nUSP: 11796594

#include "funcoesArvB.h"
#include "funcoesLeitura.h"
#include "funcoesEscrita.h"
#include "funcoesFornecidas.h"

// Função que nos auxilia na escrita de cada um dos campos do cabecalho da arvore, alocando lixo no fim
void escreve_cabecalho_arvore(FILE *fp, cabecalho_arvB *cabecalho) {
    fseek(fp, 0, SEEK_SET);
    fwrite(&cabecalho->status, sizeof(char), 1, fp);
    fwrite(&cabecalho->noRaiz, sizeof(int), 1, fp);
    fwrite(&cabecalho->RRNproxNo, sizeof(int), 1, fp);
    
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
        fwrite(&no.P[i], sizeof(int), 1, fp);
        fwrite(&no.C[i], sizeof(int), 1, fp);
        fwrite(&no.Pr[i], sizeof(long long), 1, fp);
    }

    fwrite(&no.P[ordem_arvB-1], sizeof(int), 1, fp);
}

// Função responsável por ler página de disco
void le_no_arvore(FILE *fp, no_arvB *no) {
    fread(&no->folha, sizeof(char), 1, fp);
    fread(&no->nroChavesIndexadas, sizeof(int), 1, fp);
    fread(&no->RRNdoNo, sizeof(int), 1, fp);

    for(int i=0; i<ordem_arvB-1; i++) {
        fread(&no->P[i], sizeof(int), 1, fp);
        fread(&no->C[i], sizeof(int), 1, fp);
        fread(&no->Pr[i], sizeof(long long), 1, fp);
    }

    fread(&no->P[ordem_arvB-1], sizeof(int), 1, fp);
}

// Função que cria nó da árvore
// Responsável por preencher as paginas de disco 
no_arvB *cria_no(FILE *fp_index, cabecalho_arvB *cabecalho) {

    no_arvB *no = (no_arvB*)malloc(sizeof(no_arvB));
    no->folha = '1';
    no->nroChavesIndexadas = 0;
    for(int i=0; i<ordem_arvB-1; i++) {
        no->C[i] = -1;
        no->Pr[i] = -1;
        no->P[i] = -1;
    }
    no->P[ordem_arvB-1] = -1;

    fseek(fp_index, (cabecalho->RRNproxNo + 1)*77, SEEK_SET);

    escreve_no_arvore(fp_index, *no);
    cabecalho->RRNproxNo++;

    return no;
}

// Função que cria indice para árvore
// Recebe como parametro:
// os ponteiros fp_bin (ponteiro do arquivo de registros), fp_index (ponteiro da árvore)
// e tipo (se linha ou veiculo)
void cria_arvB(FILE *fp_bin, FILE *fp_index, int tipo) {

    // Inicializando a arvore
    cabecalho_arvB *cabecalho_arv = (cabecalho_arvB*) malloc(sizeof(cabecalho_arvB));
    cabecalho_arv->status = 1;
    cabecalho_arv->noRaiz = -1;
    cabecalho_arv->RRNproxNo = 0;

    escreve_cabecalho_arvore(fp_index, cabecalho_arv);
    
    // Se for veiculo, le a quantidade de registros e os insere na árvore
    if(tipo == 9) {

        // Aloca espaço para o cabecalho, lê ele e pega a quantidade de registros que tem no arquivo binário
        cabecalho_veiculo *cabecalho = (cabecalho_veiculo*) malloc(sizeof(cabecalho_veiculo));
        le_cabecalho_veiculo(fp_bin, cabecalho);
        int qtd_registros = cabecalho->nroRegistros;

        dados_veiculo *dados = (dados_veiculo*) malloc(sizeof(dados_veiculo));
        
        // le cada um dos registros de veiculo e insere na arvore
        for(int i=0; i <qtd_registros; i++) {

            // Lê o byteoffset do registro 
            int offset = ftell(fp_bin);

            // Lê os dados do registro
            recebe_dados_veiculo(fp_bin, dados); 

            // Converte a chave para int
            int chave = convertePrefixo(dados->prefixo);

            //Se o registro não estiver marcado como removido, inserimos na árvore
            //if(dados->removido != '0') insere_no(fp_index, chave, offset, cabecalho_arv);
        }

        free(cabecalho);
        free(dados);
    }

    else if(tipo == 10) {
        
        // Aloca espaço para o cabecalho, lê ele e pega a quantidade de registros que tem no arquivo binário
        cabecalho_linha *cabecalho = (cabecalho_linha*) malloc(sizeof(cabecalho_linha));
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
           // if(dados->removido != '0') insere_no(fp_index, dados->codLinha, offset, cabecalho_arv);
        }

        free(cabecalho);
        free(dados);
    }
}


// Função recursiva da busca
int busca(int RRN, int *byteoffset, int *RRN_encontrado, int chave, FILE *fp_index) {

    //printf("RRN ATUAL: %d\n", RRN);

    // Chave de busca não encontrada / Caso Base
    if(RRN == -1) return 0;
    
    // Posiciona o ponteiro na pagina de disco desejada e a coloca na RAM
    fseek(fp_index, (RRN+1)*77, SEEK_SET);
    no_arvB *no = (no_arvB*) malloc(sizeof(no_arvB));
    le_no_arvore(fp_index, no);

    //printf("C[0] = %d, C[1] = %d, C[2] = %d, C[3] = %d\n", no->C[0],no->C[1],no->C[2],no->C[3]);

    // Pesquisa na página, procurando a chave de busca
    for(int i = 0; i < ordem_arvB-1; i++) {

        // Se encontrou, então armazenamos seu byteoffset e saímos
        if(chave == no->C[i]) {
            *byteoffset = no->Pr[i];
            free(no);
            //printf("BYTEOFFSET: %d\n\n", *byteoffset);
            return 1;
        }

        // Se a chave buscada é menor que a chave[i] ou , entao devemos descer no rrn exatamente anterior a chave[i]
        if(chave < no->C[i] || no->C[i] == -1) return busca(no->P[i], byteoffset, RRN_encontrado, chave, fp_index);
    
        // Se entrou no if, então a chave buscada é maior que todas as outras chaves no nó, então usamos o último rrn da página
        if(i == ordem_arvB-2)  return busca(no->P[ordem_arvB-1], byteoffset, RRN_encontrado, chave, fp_index);
        
    }

    free(no);
    return 0;
}

// Função que encontra o registro que contém a chave (valor) passada
void busca_dados_indice(FILE *fp_bin, FILE *fp_index, int valor, int tipo) {

    cabecalho_arvB *cabecalho = (cabecalho_arvB*) malloc(sizeof(cabecalho_arvB));
    le_cabecalho_arvore(fp_index, cabecalho);
    
    int encontrou, byteoffset = 0, RRN_anterior = 0;
    encontrou = busca(cabecalho->noRaiz, &byteoffset, &RRN_anterior, valor, fp_index);
    
    // Se encontramos a chave associada ao valor passado, então
    if(encontrou) {

        // Se tipo for veiculo 
        if(tipo == 11) {
            
            cabecalho_veiculo *cabecalho_v = (cabecalho_veiculo*) malloc(sizeof(cabecalho_veiculo));
            le_cabecalho_veiculo(fp_bin, cabecalho_v);

            // Posiciona o ponteiro no registro que possui a chave encontrada
            fseek(fp_bin, byteoffset+5, SEEK_SET);
            dados_veiculo *dados = (dados_veiculo*) malloc(sizeof(dados_veiculo));

            // Lê os valores do registro e printa na tela
            recebe_dados_veiculo(fp_bin, dados);
            printa_veiculo(dados, cabecalho_v);

            free(dados->modelo);
            free(dados->categoria);
            dados->modelo = NULL;
            dados->categoria = NULL;

            free(dados);
            free(cabecalho_v);
        }

        // Se tipo for linha
        else if(tipo == 12) {

            cabecalho_linha *cabecalho_l = (cabecalho_linha*)malloc(sizeof(cabecalho_linha));
            le_cabecalho_linha(fp_bin, cabecalho_l);
                
            // Então posiciona o ponteiro no registro que possui a chave encontrada
            fseek(fp_bin, byteoffset+5, SEEK_SET);
            dados_linha *dados = (dados_linha*) malloc(sizeof(dados_linha));


            // Lê os valores do registro e printa na tela
            recebe_dados_linha(fp_bin, dados);
            printa_linha(dados, cabecalho_l);

            free(dados->nomeLinha);
            free(dados->corLinha);
            dados->nomeLinha = NULL;
            dados->corLinha = NULL;

            free(dados);
            free(cabecalho_l);
        }
    }

    // Se o byteoffset não foi encontrado, printa "registro inexistente"
    else printf("Registro inexistente.\n");
    
    free(cabecalho);
}

/*
// Função que insere a chave no arquivo de indices da arvore b
void insere_no(FILE *fp_index, int chave, int byteoffset, cabecalho_arvB *cabecalho_arv) {

    if(cabecalho_arv->noRaiz == -1){
        // Cria o primeiro nó da arvore
        cria_no(fp_index, cabecalho_arv);

        // RRN*1 = posicao 77, nao é 0 pq 0*1 = posicao 0 e esse é o lugar onde comeca o cabecalho
        cabecalho_arv->noRaiz == 1;
    }

    // Armazenará o RRN da página na qual deveremos inserir a chave
    int RRN; 

    // Se o retorno da busca == 0, então não encontramos a chave na árvore e devemos inserí-la
    if(!busca(cabecalho_arv->noRaiz, 0, &RRN, chave, fp_index)){

        // Pula para a página na qual tentaremos fazer a inserção
        fseek(fp_index, RRN, SEEK_SET);

        no_arvB *no = (no_arvB*) malloc(sizeof(char)*77);
        le_no_arvore(fp_index, no);

        // Basta deslocarmos os elementos de modo a armazenarmos nossa chave na posicao correta
        if(no->nroChavesIndexadas < ordem_arvB-1){

            // Encontra a posicao na qual devemos inserir a chave e insere
            for(int i=0; i<ordem_arvB-1; i++) {
               
            }
        }

        // Necessário realizar split com possível promoção
        else{
            split(); // dentro dessa funcao que fariamos todos os tratamentos de split, promocao etc, com chamadas
                     // recursivas dela mesma 
        }

    }

    else {
        printf("Chave já inseridaaaaaaaa\n"); 
    }

}

/*
    current_RRN: RRN da página da árvore-B que está atualmente em uso (inicialmente, a raiz)
    key: chave a ser inserida
    promo_key: retorna a chave promovida, caso a inserção resulte no particionamento e na promoção da chave

void insert(int RRN_atual, int RRN, int key, int chave_promovida, int promo_r_child, FILE *fp_index) {

    if(RRN_atual == -1) {
        int promo_key = key;
        int promo_r_child = -1;
        int promo_RRN = RRN;

        return -1;
    }

    no_arvB *no = (no_arvB*)malloc(sizeof(no_arvB));
    le_no_arvore(fp_index, no);
}*/