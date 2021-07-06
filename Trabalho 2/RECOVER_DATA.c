//Laís Saloum Deghaide, nUSP: 11369767
//Thiago Henrique dos Santos Cardoso, nUSP: 11796594

#include "RECOVER_DATA.h"
#include "funcoesFornecidas.h"
#include "funcoesLeitura.h"
#include "constants.h"

void RECOVER_DATA(int c) {

    // Lê nome do csv que será lido e nome do binário que será criado
    char nome_bin[30], nome_index[30];
    scanf(" %s", nome_bin);
    scanf(" %s", nome_index);

    // Checando se tem falha na abertura dos arquivos
    FILE *fp_bin, *fp_index;
    if(!abertura_arquivo(&fp_bin, &fp_index, nome_bin, nome_index, "rb", "rb")) return;
    
    if(c == 11) {
        char prefixo[8], valor[15];
        scanf("%s", prefixo);
        scan_quote_string(valor);
        int valor_chave = convertePrefixo(valor);
        busca_dados_indice(fp_bin, fp_index, valor_chave, c);
    }

    else if(c == 12) {
        char codLinha[10];
        int valor;
        scanf("%s", codLinha);
        scanf("%d", &valor);
        busca_dados_indice(fp_bin, fp_index, valor, c);
    }

    fclose(fp_bin);
    fclose(fp_index);
}