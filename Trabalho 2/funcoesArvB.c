//Laís Saloum Deghaide, nUSP: 11369767
//Thiago Henrique dos Santos Cardoso, nUSP: 11796594

#include "funcoesArvB.h"
#include "funcoesLeitura.h"
#include "funcoesEscrita.h"
#include "funcoesFornecidas.h"

/* DUVIDAADASDASDAS Linhas: 68 - 99 - 205 - 230 - 249 - 253 - 266 - 270*/

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

/*

LEIA AQUI :

Não sei se entendi direito a busca, porque as chaves dos nós estão completamente desordenadas (rode programa com caso 5.in)
Também não sei se a conversão feita está correta... TOMAR NO CU DESSA PORRA DE MATERIA
Enfim, não entendi direito as coisas.

*/

// Função recursiva da busca
int busca(int RRN, int *byteoffset, int *RRN_anterior, int chave, FILE *fp_index) {

    // Armazeno o RRN da página de disco anterior a atual
    *RRN_anterior = RRN;

    // Chave de busca não encontrada / Caso Base
    if(RRN == -1) return 0;
    
    // Posiciona o ponteiro na pagina de disco desejada e a coloca na RAM
    fseek(fp_index, (RRN+1)*77, SEEK_SET);
    no_arvB *no = (no_arvB*) malloc(sizeof(no_arvB));
    le_no_arvore(fp_index, no);

    // Printa as chaves da página para fins de debug
    printf("C[0] = %d, C[1] = %d, C[2] = %d, C[3] = %d\n", no->C[0],no->C[1],no->C[2],no->C[3]);

    // Pesquisa na página, procurando a chave de busca
    for(int i = 0; i < ordem_arvB-1; i++) {

        // Se encontrou, então armazenamos seu byteoffset e saímos
        if(chave == no->C[i]) {
            *byteoffset = no->Pr[i];
            free(no);
            return 1;
        }
    }

    // Se não, acha o próximo RRN para continuar a pesquisa
    for(int i = 0; i < ordem_arvB-1; i++) {

        // Se a chave buscada é menor que a chave[i], entao devemos descer no rrn exatamente anterior a chave[i]
        if(chave < no->C[i]) {
            RRN = no->P[i];
            break;
        }

        // Se entrou no if, então a chave buscada é maior que todas as outras chaves no nó, então usamos o último rrn da página
        if(i == ordem_arvB-2) {
            RRN = no->P[ordem_arvB-1];
        }

    }

    free(no);
    return busca(RRN, byteoffset, RRN_anterior, chave, fp_index);
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

            /* DUVIDA: Por que 175? Nao seria fseek pro offset direto? */
            // Então posiciona o ponteiro no registro que possui a chave encontrada
            fseek(fp_bin, byteoffset, SEEK_SET);

            /* DUVIDA: Falta ler o cabecalho do veiculo, nao? */
            // Aloca espaço pro cabecalho e para os dados do veiculo
            dados_veiculo *dados = (dados_veiculo*)malloc(sizeof(dados_veiculo));
            cabecalho_veiculo *cabecalho_v = (cabecalho_veiculo*) malloc(sizeof(cabecalho_veiculo));

            // Lê os valores do registro e printa na tela
            recebe_dados_veiculo(fp_bin, dados);
            printa_veiculo(dados, cabecalho_v);
        }

        // Se tipo for linha
        else if(tipo == 12) {

            /* DUVIDA: Por que 83? Nao seria fseek pro offset direto? */
            // Então posiciona o ponteiro no registro que possui a chave encontrada
            fseek(fp_bin, 83*(byteoffset+1), SEEK_SET); // CHECAR TAMANHO DO CABECALHO DE LINHA

            /* DUVIDA: Falta ler o cabecalho da linha, nao? */
            // Aloca espaço pro cabecalho e para os dados do veiculo
            dados_linha *dados = (dados_linha*)malloc(sizeof(dados_linha));
            cabecalho_linha *cabecalho_l = (cabecalho_linha*)malloc(sizeof(cabecalho));

            // Lê os valores do registro e printa na tela
            recebe_dados_linha(fp_bin, dados);
            printa_linha(dados, cabecalho_l);
        }
    }

    // Se o byteoffset não foi encontrado, printa "registro inexistente"
    else {
         printf("Registro inexistente.\n");
    }
    
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