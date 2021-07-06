//Laís Saloum Deghaide, nUSP: 11369767
//Thiago Henrique dos Santos Cardoso, nUSP: 11796594

#include "LOOP_UNICO.h"

void LOOP_UNICO(int c) {

    // Lê nome os nomes dos arquivos e dos campos que vão ser procurados
    char nome_veiculo[30], nome_linha[30], codLinha_v[15], codLinha_l[15], indiceLinha[15];
    int indiceLinha;
    scanf(" %s", nome_veiculo);
    scanf(" %s", nome_linha);
    scanf(" %s", codLinha_v);
    scanf(" %s", codLinha_l);    
    scanf(" %s", indiceLinha);

    // Checando se tem falha na abertura dos arquivos
    FILE *fp_v, *fp_l, *fp_index;
    if(!abertura_arquivo(&fp_v, nome_veiculo, "rb")) return;
    if(!abertura_arquivo(&fp_l, nome_linha, "rb")) return;
    if(!abertura_arquivo(&fp_index, indiceLinha, "rb")) return;

    cabecalho_veiculo *cabecalho_v = le_cabecalho_veiculo(fp_v);

    // Checa o caso de não haver registros no arquivo de veiculo e, portanto, não terá junção
    if(cabecalho_v->nroRegistros == 0) {
        printf("Registro inexistente.\n");
        return;
    }

    dados_veiculo *dados_v = (dados_veiculo*) malloc(sizeof(dados_veiculo));

    // Percorre registros de linha
    while(fread(&dados_v->removido, sizeof(char), 1, fp_v) != 0) {
        fread(&dados_v->tamanhoRegistro, sizeof(int), 1, fp_v);

        //Se o registro estiver marcado como removido, pulamos ele
        if(dados_v->removido == '0') fseek(fp_v, dados_v->tamanhoRegistro, SEEK_CUR);

        // Passa o valor para a função de busca
        busca_dados_indice(fp_l, fp_index, dados_v->codLinha);
        
        free(dados_v->modelo);
        free(dados_v->categoria);
        dados_v->modelo = NULL;
        dados_v->categoria = NULL;
    }

    free(fp_v);
    free(fp_l);
    free(fp_index);
    free(dados_v);
    free(cabecalho_v);
}