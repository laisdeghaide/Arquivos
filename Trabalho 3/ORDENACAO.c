//Laís Saloum Deghaide, nUSP: 11369767
//Thiago Henrique dos Santos Cardoso, nUSP: 11796594

#include "ORDENACAO.h"

void ORDENACAO(int c) {
    
    // Lê nome os nomes dos arquivos e dos campos que vão ser procurados
    char arquivoDesordenado[30], arquivoOrdenado[30], campoOrdenacao[15];
    scanf(" %s", arquivoDesordenado);
    scanf(" %s", arquivoOrdenado);
    scanf(" %s", campoOrdenacao);

    FILE *fp_desord, *fp_ord;
    if(!abertura_arquivo(&fp_desord, arquivoDesordenado, "rb")) return;
    if(!abertura_arquivo(&fp_ord, arquivoOrdenado, "wb")) return;
}