//Laís Saloum Deghaide, nUSP: 11369767
//Thiago Henrique dos Santos Cardoso, nUSP: 11796594

#include "ORDENACAO.h"

void ORDENACAO() {

    // Lê nome os nomes dos arquivos e dos campos que vão ser procurados
    char arq_desordenado[30], arq_ordenado[50], campo_ordenacao[30];
    scanf(" %s", arq_desordenado);
    scanf(" %s", arq_ordenado);
    scanf(" %s", campo_ordenacao);

    // Checando se tem falha na abertura dos arquivos
    FILE *fp_ord, *fp_des;
    if(!abertura_arquivo(&fp_des, arq_desordenado, "rb")) return;
    if(!abertura_arquivo(&fp_ord, arq_ordenado, "rb")) return;

    cabecalho_veiculo *cabecalho_v = le_cabecalho_veiculo(fp_des);

    // Checa o caso de não haver registros no arquivo de veiculo e, portanto, não tem o que ordenar
    if(cabecalho_v->nroRegistros == 0 || cabecalho_v->status == '0') {
        printf("Falha no processamento do arquivo.\n");
        free(cabecalho_v);
        return;
    }

    // Aloco uma matriz na qual cada linha representará um dos dados do veículo
    dados_veiculo **dados_ram = (dados_veiculo**) malloc(sizeof(dados_veiculo*));

    /* A PARTIR DAQUI, CRIAR UMA FUNCAO A PARTE QUE REALIZA ESSA ORDENACAO */

    // (i) Passa para a RAM todo o arquivo de dados desordenado
    for(int i = 0 ; i < cabecalho_v->nroRegistros ; i++) {

        dados_ram[i] = (dados_veiculo*) malloc(sizeof(dados_veiculo));
        
        if(fread(&dados_ram[i]->removido, sizeof(char), 1, fp_des) !=0) break;
        fread(&dados_ram[i]->tamanhoRegistro, sizeof(int), 1, fp_des);

        // Se o registro estiver marcado como removido, pulamos ele
        if(dados_ram[i]->removido == '0') {
            fseek(fp_des, dados_ram[i]->tamanhoRegistro, SEEK_CUR);
            free(dados_ram[i]);
            i--; // Subtraio um de i pois para registros removidos, não devemos computar
        }

        else recebe_dados_veiculo(fp_des, dados_ram[i]);
    }

    fclose(fp_des);

    // (ii) Realiza a ordenação desses dados armazenados na matriz dados_ram


    // (iii) Escreve os dados ordenados para um arquivo de saída apontado por fp_ord
    escreve_cabecalho_veiculo(fp_ord, *cabecalho_v);

    for(int i = 0 ; i < cabecalho_v->nroRegistros ; i++) {
        escreve_dados_veiculo(fp_ord, dados_ram[i]);
        free(dados_ram[i]);
    }

    free(dados_ram);
    /* FUNCAO ACABA AQUI */

    free(cabecalho_v);
    fclose(fp_ord);
    binarioNaTela(arq_ordenado);
}