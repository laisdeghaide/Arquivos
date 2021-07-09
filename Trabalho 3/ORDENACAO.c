//Laís Saloum Deghaide, nUSP: 11369767
//Thiago Henrique dos Santos Cardoso, nUSP: 11796594

#include "ORDENACAO.h"

void ORDENACAO(int c) {

    // Lê nome os nomes dos arquivos e dos campos que vão ser procurados
    char arq_desordenado[30], arq_ordenado[50], campo_ordenacao[30];
    scanf(" %s", arq_desordenado);
    scanf(" %s", arq_ordenado);
    scanf(" %s", campo_ordenacao);

    // Checando se tem falha na abertura dos arquivos
    FILE *fp_ord, *fp_des;
    if(!abertura_arquivo(&fp_des, arq_desordenado, "rb")) return;
    if(!abertura_arquivo(&fp_ord, arq_ordenado, "rb")) return;

    if(c == 17){

        cabecalho_veiculo *cabecalho_v = le_cabecalho_veiculo(fp_des);

        if(cabecalho_v->status == '0') {
            printf("Falha no processamento do arquivo.\n");
            free(cabecalho_v);
            return;
        }

        if(cabecalho_v->nroRegistros == 0 ) {
            printf("Registro inexistente.\n");
            free(cabecalho_v);
            return;
        }

        ordena_arquivo(fp_ord, fp_des, c, NULL, cabecalho_v);
        free(cabecalho_v);

    }

    if(c == 18){
        
        cabecalho_linha *cabecalho_l = le_cabecalho_linha(fp_des);

        if(cabecalho_l->status == '0') {
            printf("Falha no processamento do arquivo.\n");
            free(cabecalho_l);
            return;
        }

        if(cabecalho_l->nroRegistros == 0) {
            printf("Registro inexistente.\n");
            free(cabecalho_l);
            return;
        }

        ordena_arquivo(fp_ord, fp_des, c, cabecalho_l, NULL);
        free(cabecalho_l);
    }

    fclose(fp_ord);
    binarioNaTela(arq_ordenado);
}