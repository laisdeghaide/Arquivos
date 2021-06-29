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
void cria_no(no_arvB *no) {

    no->folha = '1';
    no->nroChavesIndexadas = 0;
    for(int i=0; i<ordem_arvB-1; i++) {
        no->P[i] = -1;
        no->C[i] = -1;
        no->Pr[i] = -1;
    }
    no->P[ordem_arvB-1] = -1;

    /*fseek(fp_index, (cabecalho->RRNproxNo + 1)*77, SEEK_SET);

    escreve_no_arvore(fp_index, *no);
    cabecalho->RRNproxNo++;
    */
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

    // Chave de busca não encontrada / Caso Base
    if(RRN == -1) return 0;
    
    // Posiciona o ponteiro na pagina de disco desejada e a coloca na RAM
    fseek(fp_index, (RRN+1)*77, SEEK_SET);
    no_arvB *no = (no_arvB*) malloc(sizeof(no_arvB));
    le_no_arvore(fp_index, no);

    // Pesquisa na página, procurando a chave de busca
    for(int i = 0; i < ordem_arvB-1; i++) {

        // Se encontrou, então armazenamos seu byteoffset e saímos
        if(chave == no->C[i]) {
            *byteoffset = no->Pr[i];
            free(no);
            return 1;
        }

        // Se a chave buscada é menor que a chave[i] ou chave vazia, entao devemos descer no rrn exatamente anterior a chave[i]
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


// Função recursiva da inserção
/*
RRN_atual: RRN da página da árvore-B que está atualmente em uso (inicialmente, a raiz)
chave: chave a ser inserida
chave_promo: retorna a chave promovida, caso a inserção resulte no particionamento e na promoção da chave
filho_promo: nó filho que será promovido
*/
int insere(int RRN_atual, int byteoffset, int chave, int *chave_promo, int *filho_promo, FILE *fp_index, cabecalho_arvB *cabecalho_arv) {
    
    // VARIÁVEIS LOCAIS:
    no_arvB *pagina; // página de disco nova resultante do particionamento
    no_arvB *nova_pagina; // página de disco correntemente examinada pela função 
    int pos = 0; // posição na página na qual a chave ocorre ou deveria ocorrer
    int p_b_chave; // chave promovida do nível inferior para ser inserida na página
    int p_b_rrn; // RRN promovido do nível inferior para ser inserido na página, filho à direita de P_B_CHAVE

    cria_no(pagina);
    cria_no(nova_pagina);

    // Se chegar em um nó folha, então é necessário inserir em um nó acima, fazendo a promoção
    if(RRN_atual == -1) {
        *chave_promo = chave;
        *filho_promo = -1;

        return 1;
    }

    // Se a página não é um nó folha, a função é chamada recursivamente até que ela encontre uma chave ou chegue no nó folha
    // Pesquisa a página procurando a chave de busca
    for(int i=0; i<ordem_arvB-1; i++) {
        pos = i;

        // Se chave de busca já existe
        if(chave == pagina->C[i]) return -1;

        // Se a chave está em um nó filho, então encerra o laço e procura recursivamente a posicao desejada para insercao no nó filho
        if(chave < pagina->C[i]) break;

        // Se a chave é maior que todas as chaves da página em que estamos
        if(i == ordem_arvB-2) pos++;
    }

    int insercao = insere(pagina->P[pos], byteoffset, chave, &p_b_chave, &p_b_rrn, fp_index, cabecalho_arv);

    // Se insercao == 0, então não é necessário promoção
    // Se insercao == -1, então aconteceu algum erro
    if(insercao == 0 || insercao == -1) return insercao;

    // Se ainda tem espaço no nó, então insere sem promoção
    else if(pagina->nroChavesIndexadas < ordem_arvB-1) {
        int pos2 = pagina->nroChavesIndexadas;

        // Shiftando todas as posições até encontrar a posição certa para inserção
        while(pos2 >= 0 && pagina->C[pos2-1] > chave) {
            pagina->P[pos2 + 1] = pagina->P[pos2];
            pagina->C[pos2] = pagina->C[pos2 - 1];
            pagina->Pr[pos2] = pagina->Pr[pos2 - 1];

            pos2--;
        }

        // Inserindo
        pagina->P[pos2 + 1] = p_b_rrn;
        pagina->C[pos2] = p_b_chave;
        pagina->Pr[pos2] = byteoffset;
        pagina->nroChavesIndexadas++;

        fseek(fp_index, (RRN_atual+1)*77, SEEK_SET);
        escreve_no_arvore(fp_index, *pagina);

        // Sem promoção
        return 0;
    }

    // É necessário realizar split e promover
    else {
        split();

        fseek(fp_index, (RRN_atual+1)*77, SEEK_SET);
        escreve_no_arvore(fp_index, *pagina);
        
        fseek(fp_index, (cabecalho_arv->RRNproxNo+1)*77, SEEK_SET);
        escrever_node(fp_index, *nova_pagina);

        cabecalho_arv->RRNproxNo++;

        return 1;
    }
}

// Função que insere a chave no arquivo de indices da arvore b
// Procedimento Driver
int insere_no(FILE *fp_index, int chave, int byteoffset, cabecalho_arvB *cabecalho_arv) {

    // Cria o primeiro nó da arvore
    if(cabecalho_arv->noRaiz == -1){
        no_arvB *no;
        cria_no(no);
        no->folha = '1';
        no->nroChavesIndexadas = 1;
        no->C[0] = chave;
        no->Pr[0] = byteoffset;

        escreve_no_arvore(fp_index, *no);

        cabecalho_arv->noRaiz = 1;
        cabecalho_arv->RRNproxNo = 2;
    }

    // Se árvore já existe, então vamos inserir 
    else {
        int chave_promo, filho_promo;

        // Insere recursivamente
        int insercao = insere(cabecalho_arv->noRaiz, byteoffset, chave, &chave_promo, &filho_promo, fp_index, cabecalho_arv);

        // Se for necessário promoção, então cria nova raiz e a preenche
        if(insercao == 1) {
            no_arvB *novo_no;
            cria_no(novo_no);

            novo_no->nroChavesIndexadas = 1;
            novo_no->folha = '1';
            novo_no->P[0] = cabecalho_arv->noRaiz;
            novo_no->C[0] = chave_promo;
            novo_no->Pr[0] = byteoffset;
            novo_no->P[1] = filho_promo;

            // Atualizando o cabeçalho
            cabecalho_arv->noRaiz = cabecalho_arv->RRNproxNo;
            cabecalho_arv->RRNproxNo++;

            // Escrevendo a nova página na árvore
            escreve_no_arvore(fp_index, *novo_no);
        }

        // Se houve erro
        else if(insercao == -1) return -1;
    }

    // Se não foi necessário nenhuma promoção/split
    return 1;
}

//void split()



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
*/