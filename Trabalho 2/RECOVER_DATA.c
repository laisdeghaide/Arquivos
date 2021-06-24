//Laís Saloum Deghaide, nUSP: 11369767
//Thiago Henrique dos Santos Cardoso, nUSP: 11796594

#include "RECOVER_DATA.h"
#include "funcoesFornecidas.h"
#include "constants.h"

void RECOVER_DATA(int c) {

    // Lê nome do csv que será lido e nome do binário que será criado
    char nome_bin[30], nome_index[30];
    scanf(" %s", nome_bin);
    scanf(" %s", nome_index);

    // Checa caso de falha na abertura dos arquivos
    FILE *fp_bin = fopen(nome_bin, "rb");
    if (fp_bin == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }   

    FILE *fp_index = fopen(nome_index, "rb");
    if (fp_index == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }   
    
    int valor;
    if(c == 11) {
        char prefixo[20];
        char valor[6];
        scanf("%s %s", prefixo, valor);

        int valor_chave = convertePrefixo(valor);

        busca_dados_indice(fp_bin, fp_index, valor_chave, c);
    }

    else if(c == 12) {
        char codLinha[20];
        int valor;
        scanf("%s %d", codLinha, &valor);

        busca_dados_indice(fp_bin, fp_index, valor, c);
    }

    fclose(fp_bin);
    fclose(fp_index);
}