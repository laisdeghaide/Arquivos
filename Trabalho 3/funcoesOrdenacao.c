//Laís Saloum Deghaide, nUSP: 11369767
//Thiago Henrique dos Santos Cardoso, nUSP: 11796594

#include "funcoesOrdenacao.h"

int comparadorRegistrosVeiculo(const void *p1, const void *p2){
    if((*(dados_veiculo**)p1)->codLinha < (*(dados_veiculo**)p2)->codLinha) return -1;
    if((*(dados_veiculo**)p1)->codLinha == (*(dados_veiculo**)p2)->codLinha) return 0;
    if((*(dados_veiculo**)p1)->codLinha > (*(dados_veiculo**)p2)->codLinha) return 1;
}

int comparadorRegistrosLinha(const void *p1, const void *p2){
    if((*(dados_linha**)p1)->codLinha < (*(dados_linha**)p2)->codLinha) return -1;
    if((*(dados_linha**)p1)->codLinha == (*(dados_linha**)p2)->codLinha) return 0;
    if((*(dados_linha**)p1)->codLinha > (*(dados_linha**)p2)->codLinha) return 1;
}

void ordena_arquivo(FILE *fp_ord, FILE *fp_des, int tipo, cabecalho_linha *cabecalho_l, cabecalho_veiculo *cabecalho_v){

    // Se quisermos ordenar um arquivo do tipo veículo
    if(tipo == 17){

        // Aloco uma matriz na qual cada linha representará um dos dados do veículo
        dados_veiculo **dados_ram = (dados_veiculo**) malloc(sizeof(dados_veiculo*));
        dados_ram[0] = (dados_veiculo*) malloc(sizeof(dados_veiculo));

        // (i) Passa para a RAM todo o arquivo de dados desordenado
        for(int i = 0 ; i < cabecalho_v->nroRegistros ; i++) {

            fread(&dados_ram[i]->removido, sizeof(char), 1, fp_des);
            fread(&dados_ram[i]->tamanhoRegistro, sizeof(int), 1, fp_des);

            // Se o registro estiver marcado como removido, pulamos ele
            if(dados_ram[i]->removido == '0') {
                fseek(fp_des, dados_ram[i]->tamanhoRegistro, SEEK_CUR);
                i--; // Subtraio um de i pois para registros removidos, não devemos computar
            }

            else {
                recebe_dados_veiculo(fp_des, dados_ram[i]);
                //printa_veiculo(dados_ram[i], cabecalho_v);
                dados_ram[i+1] = (dados_veiculo*) malloc(sizeof(dados_veiculo));
            }

        }
        //fclose(fp_des);

        /*
        int num = 0;
        for(int i = 0 ; i < cabecalho_v->nroRegistros; i++){
            printa_veiculo(dados_ram[i], cabecalho_v);
            num++;
        }
        printf("nroReg: %d\t num: %d\n", cabecalho_v->nroRegistros, num);
        */
       
        // (ii) Realiza a ordenação desses dados armazenados na matriz dados_ram
        qsort(dados_ram, cabecalho_v->nroRegistros, sizeof(dados_veiculo*), comparadorRegistrosVeiculo);

        printf("ordenou c sucesso\n");

        // (iii) Escreve os dados ordenados para um arquivo de saída apontado por fp_ord
        escreve_cabecalho_veiculo(fp_ord, *cabecalho_v);

        for(int i = 0 ; i < cabecalho_v->nroRegistros ; i++) {
            escreve_dados_veiculo(fp_ord, dados_ram[i]);
            //free(dados_ram[i]);
        }

        printf("passou de volta pro arquivo direito\n");

        free(dados_ram);
    }

    // Se quisermos ordenar um arquivo do tipo linha
    if(tipo == 18){

         // Aloco uma matriz na qual cada linha representará um dos dados dda linha
        dados_linha **dados_ram = (dados_linha**) malloc(sizeof(dados_linha*));

        // (i) Passa para a RAM todo o arquivo de dados desordenado
        for(int i = 0 ; i < cabecalho_l->nroRegistros ; i++) {

            dados_ram[i] = (dados_linha*) malloc(sizeof(dados_linha));
            
            if(fread(&dados_ram[i]->removido, sizeof(char), 1, fp_des) !=0) break;
            fread(&dados_ram[i]->tamanhoRegistro, sizeof(int), 1, fp_des);

            // Se o registro estiver marcado como removido, pulamos ele
            if(dados_ram[i]->removido == '0') {
                fseek(fp_des, dados_ram[i]->tamanhoRegistro, SEEK_CUR);
                free(dados_ram[i]);
                i--; // Subtraio um de i pois para registros removidos, não devemos armazenar
            }

            else recebe_dados_linha(fp_des, dados_ram[i]);
        }
        fclose(fp_des);

        // (ii) Realiza a ordenação desses dados armazenados na matriz dados_ram
        qsort(dados_ram, cabecalho_v->nroRegistros, sizeof(dados_veiculo*), comparadorRegistrosLinha);

        // (iii) Escreve os dados ordenados para um arquivo de saída apontado por fp_ord
        escreve_cabecalho_linha(fp_ord, *cabecalho_l);

        for(int i = 0 ; i < cabecalho_v->nroRegistros ; i++) {
            escreve_dados_linha(fp_ord, dados_ram[i]);
            free(dados_ram[i]);
        }

        free(dados_ram);
        free(cabecalho_v);
    }
}
