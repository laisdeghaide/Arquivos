//Laís Saloum Deghaide, nUSP: 11369767
//Thiago Henrique dos Santos Cardoso, nUSP: 11796594

#include "MERGE.h"

void MERGE() {

    // Lê nome os nomes dos arquivos e dos campos que vão ser procurados
    char nome_veiculo[30], nome_linha[30], nomeCampoVeiculo[15], nomeCampoLinha[15];
    scanf(" %s", nome_veiculo);
    scanf(" %s", nome_linha);
    scanf(" %s", nomeCampoVeiculo);   
    scanf(" %s", nomeCampoLinha);

    // Checando falhas na abertura dos arquivos
    FILE *fp_v, *fp_l;
    if(!abertura_arquivo(&fp_v, nome_veiculo, "rb")) return;
    if(!abertura_arquivo(&fp_l, nome_linha, "rb")) return;

    cabecalho_veiculo *cabecalho_v = le_cabecalho_veiculo(fp_v);
    cabecalho_linha *cabecalho_l = le_cabecalho_linha(fp_l);
        
    if(cabecalho_v->nroRegistros == 0 || cabecalho_l->nroRegistros == 0) {
        printf("Registro Inexistente.\n");
        free(cabecalho_l);
        free(cabecalho_v);
        return;
    }

    if(cabecalho_v->status == '0' || cabecalho_l->status == '0') {
        printf("Falha no processamento do arquivo.\n");
        free(cabecalho_l);
        free(cabecalho_v);        
        return;
    }

    // Criação dos arquivos nos quais teremos os dados ordenados
    FILE *fp_ord_v, *fp_ord_l;
    if(!abertura_arquivo(&fp_ord_v, "v_ord.bin", "wb+")) return;
    if(!abertura_arquivo(&fp_ord_l, "l_ord.bin", "wb+")) return;

    // Ordenação de ambos os arquivos
    ordena_arquivo(fp_ord_v, fp_v, 17, NULL, cabecalho_v);
    ordena_arquivo(fp_ord_l, fp_l, 18, cabecalho_l, NULL);
    
    // Espaco onde guardarei os registros de modo a fazer a comparaçãok
    dados_veiculo *dados_v = (dados_veiculo*) malloc(sizeof(dados_veiculo));
    dados_linha *dados_l = (dados_linha*) malloc(sizeof(dados_linha));

    // Recebe os primeiros registros
    fread(&dados_v->removido, sizeof(char), 1, fp_ord_v);
    fread(&dados_v->tamanhoRegistro, sizeof(int), 1, fp_ord_v);
    recebe_dados_veiculo(fp_ord_v, dados_v);

    fread(&dados_l->removido, sizeof(char), 1, fp_ord_l);
    fread(&dados_l->tamanhoRegistro, sizeof(int), 1, fp_ord_l);
    recebe_dados_linha(fp_ord_l, dados_l);

    // Merge dos arquivos
    bool existe_registro = false;
    while(!feof(fp_ord_v) && !feof(fp_ord_l)){

        // Se codLinha veiculo for maior que o da linha, pulo para o proximo reg no arquivo de linha e o recebo
        if(dados_v->codLinha > dados_l->codLinha){

            fread(&dados_l->removido, sizeof(char), 1, fp_ord_l);
            fread(&dados_l->tamanhoRegistro, sizeof(int), 1, fp_ord_l);
            recebe_dados_linha(fp_ord_l, dados_l);
        }

        // Se forem iguais, imprimo os registros e recebo o proximo veiculo
        if(dados_v->codLinha == dados_l->codLinha){

            existe_registro = true;
            printa_veiculo(dados_v, cabecalho_v);
            printa_linha(dados_l, cabecalho_l);

            fread(&dados_v->removido, sizeof(char), 1, fp_ord_v);
            fread(&dados_v->tamanhoRegistro, sizeof(int), 1, fp_ord_v);
            recebe_dados_veiculo(fp_ord_v, dados_v);
        }

        // Se codLinha veiculo for menor que o da linha, pulo para o proximo reg no arquivo de veiculo e o recebo
        if(dados_v->codLinha < dados_l->codLinha){

            fread(&dados_v->removido, sizeof(char), 1, fp_ord_v);
            fread(&dados_v->tamanhoRegistro, sizeof(int), 1, fp_ord_v);
            recebe_dados_veiculo(fp_ord_v, dados_v);
        }

    }

    if(!existe_registro) printf("Registro inexistente.\n");
    
    free(dados_v);
    free(dados_l);
    free(cabecalho_l);
    free(cabecalho_v);
    fclose(fp_ord_v);
    fclose(fp_ord_l);
}