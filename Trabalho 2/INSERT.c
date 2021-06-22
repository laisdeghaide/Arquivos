//Laís Saloum Deghaide, nUSP: 11369767
//Thiago Henrique dos Santos Cardoso, nUSP: 11796594

#include "RECOVER_DATA.h"
#include "funcao-fornecida.h"

void RECOVER_DATA(char c) {

    // Lê nome do csv que será lido e nome do binário que será criado
    char nome_bin[15], nome_index[15];
    scanf("%s", nome_bin);
    scanf("%s", nome_index);

    // Checa caso de falha na abertura dos arquivos
    FILE *fp_bin = fopen(nome_bin, "rb");
    if (fp_bin == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }   

    FILE *fp_index = fopen(nome_index, "wb");
    if (fp_index == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }   
    
    if(c == '13') {

    }

    else if(c == '14') {
        
    }

    fclose(fp_bin);
    fclose(fp_index);
    binarioNaTela(nome_index);
}