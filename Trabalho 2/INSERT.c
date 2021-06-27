//Laís Saloum Deghaide, nUSP: 11369767
//Thiago Henrique dos Santos Cardoso, nUSP: 11796594

#include "INSERT.h"
#include "funcoesFornecidas.h"
#include "constants.h"
#include "funcoesLeitura.h"
#include "funcoesEscrita.h"

void INSERT(int c) {

    // Lê nome do csv que será lido e nome do binário que será criado
    char nome_bin[30], nome_index[30];
    scanf(" %s", nome_bin);
    scanf(" %s", nome_index);

    // Checa caso de falha na abertura dos arquivos
    FILE *fp_bin = fopen(nome_bin, "rb+");
    if (fp_bin == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }   

    FILE *fp_index = fopen(nome_index, "rb+");
    if (fp_index == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }   
    
     // Lê a quantidade de registros a ser inserido
    int n;
    scanf("%d", &n);

    // Aloca espaço para o cabecalho de árvore e o lê
    cabecalho_arvB *cabecalho_arv = (cabecalho_arvB*) malloc(sizeof(cabecalho_arvB));
    le_cabecalho_arvore(fp_index, cabecalho_arv);

    // Se houver inconsistência no arquivo, encerra
    if(cabecalho_arv->status == '0'){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Se for veículo
    if(c == 13) {

        // Aloca espaço para o cabecalho e o lê
        cabecalho_veiculo *cabecalho_v = (cabecalho_veiculo*) malloc(sizeof(cabecalho_veiculo));
        le_cabecalho_veiculo(fp_bin, cabecalho_v);

        // Se houver inconsistência no arquivo, encerra
        if(cabecalho_v->status == '0'){
            printf("Falha no processamento do arquivo.\n");
            return;
        }

        // Inicializa o status da árvore e do veículo com 0
        cabecalho_arv->status = '0';
        cabecalho_v->status = '0';

        dados_veiculo *dados = (dados_veiculo*) malloc(sizeof(dados_veiculo));

        // Coloca o ponteiro no fim do arquivo, onde será inserido o novo registro
        fseek(fp_bin, 0, SEEK_END);

        while(n--) {
            // Insere veiculo 
            insere_veiculo(cabecalho_v, dados);

            // Insere os registros no arquivo .bin
            escreve_dados_veiculo(fp_bin, dados);

            int valor_chave = convertePrefixo(dados->prefixo);

            // Insere o indice do registro na arvore
            //insere_no(fp_index, valor_chave, cabecalho_arv->RRNproxNo, cabecalho_arv->noRaiz);

            free(dados->categoria);
            free(dados->modelo);
            dados->categoria = NULL;
            dados->modelo = NULL;
        }
        
        //Seta os arquivos como estáveis 
        cabecalho_arv->status = '1';
        cabecalho_v->status = '1';

        //Recebe a posição atual do ponteiro e após isso, o desloca para o início do arquivo
        cabecalho_v->byteProxReg = ftell(fp_bin);

        //Atualiza o cabecalho no arquivo.bin
        escreve_cabecalho_veiculo(fp_bin, *cabecalho_v);

        // Atualiza o cabecalho de indice
        escreve_cabecalho_arvore(fp_index, cabecalho_arv);

        free(dados);
        free(cabecalho_v);
    }

    // Se for linha
    else if(c == 14) {
        // Aloca espaço para o cabecalho e o lê
        cabecalho_linha *cabecalho_l = (cabecalho_linha*)malloc(sizeof(cabecalho_linha));
        le_cabecalho_linha(fp_bin, cabecalho_l);

        // Se houver inconsistência no arquivo, encerra
        if(cabecalho_l->status == '0'){
            printf("Falha no processamento do arquivo.\n");
            return;
        }

        // Inicializa o status da árvore e do veículo com 0
        cabecalho_arv->status = '0';
        cabecalho_l->status = '0';

        dados_linha *dados = (dados_linha*) malloc(sizeof(dados_linha));

        // Coloca o ponteiro no fim do arquivo, onde será inserido o novo registro
        fseek(fp_bin, 0, SEEK_END);

        while(n--) {
            // Insere veiculo 
            insere_linha(cabecalho_l, dados);

            // Insere os registros no arquivo .bin
            escreve_dados_linha(fp_bin, dados);

            // Insere o indice do registro na arvore
            //insere_no(fp_index, dados->codLinha, cabecalho_arv->RRNproxNo, cabecalho_arv->noRaiz);

            free(dados->nomeLinha);
            free(dados->corLinha);
            dados->nomeLinha = NULL;
            dados->corLinha = NULL;
        }
        
        //Seta os arquivos como estáveis 
        cabecalho_arv->status = '1';
        cabecalho_l->status = '1';

        //Recebe a posição atual do ponteiro e após isso, o desloca para o início do arquivo
        cabecalho_l->byteProxReg = ftell(fp_bin);

        //Atualiza o cabecalho no arquivo.bin
        escreve_cabecalho_linha(fp_bin, *cabecalho_l);

        // Atualiza o cabecalho de indice
        escreve_cabecalho_arvore(fp_index, cabecalho_arv);

        free(dados);
        free(cabecalho_l);
    }

    free(cabecalho_arv);
    fclose(fp_bin);
    fclose(fp_index);
    binarioNaTela(nome_index);
}