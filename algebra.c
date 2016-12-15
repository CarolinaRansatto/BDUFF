#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void selection (){

}

void projeta(char table [31], int n, char atributos[181], char exit_table[31]){

    // TODO: verificar para projeções do tipo *
    // TODO: adaptar nome dos arquivos de acordo com cada projeção de modo que todos fiquem diferentes
    // TODO: receber projeção de tuplas que podem ser nulas, adaptando o número de tuplas finais se necessário

    char table_ctl [51];
    char table_dad [51];
    char table_ctl_old [51];
    char table_dad_old [51];

    const char comma[2] = ",";
    char *token;
    char *token2;

    int posicoes[n];
    int k = 0; //contador para as posicoes

    strcpy(table_ctl, table);
    strcpy(table_ctl_old, table);
    strcpy(table_dad, table);
    strcpy(table_dad_old, table);

    strcat(table_ctl, "Proj.ctl");
    strcat(table_ctl_old, ".ctl");
    strcat(table_dad, "Proj.dad");
    strcat(table_dad_old, ".dad");

    // cria um novo arquivo ctl da projeção
    FILE* arq_ctl = fopen(table_ctl, "wt"); //criando o arquivo
    // abre arquivo dad da tabela original
    FILE* arq_ctl_old = fopen(table_ctl_old, "rt"); // ready only

    int tuplas;
    int colunas_old;
    char info [101];
    char atts [181]; // strtok_r estava quebrando a string atributos, usando essa como auxiliar...

    // não tem como usar strtok em aninhado. usando strtok_r no lugar
    char *end_token; // ponteiro pra identificação do strtok_r
    char *end_token2; // ponteiro pra identificação do strtok_r

    fscanf(arq_ctl_old, "%s", info);
    token = strtok_r(info,comma, &end_token);
    colunas_old = atoi(token);
    token = strtok_r(NULL,comma, &end_token);
    tuplas = atoi(token);

    //começa a escrever o arq_ctl
    fprintf(arq_ctl,"%d,%d\n", n,tuplas);

    // verifica se a coluna que estamos lendo do ctl antigo é a coluna que queremos projetar, depois escreve no novo ctl
    // ao mesmo tempo cria a projeção .dad do arquivo
    int i,j;
    fscanf(arq_ctl_old, "%s", info);
    token = strtok_r(info, comma, &end_token);
    for (j = 0; j < colunas_old; j++){
        strcpy(atts, atributos); //recuperando a string original
        token2 = strtok_r(atts, comma, &end_token2);
        for (i = 0; i < n; i++){
            if (token2){
                if (strcmp(token, token2) == 0){
                // grava a posição do atributo pra projetar .dad depois
                posicoes[k] = j;
                k++;
                // se os atributos tiverem o mesmo nome, escreve no arquivo
                fprintf(arq_ctl, "%s", token);
                // anda pra pegar a linha toda do arquivo e escrever no novo ctl
                token = strtok_r(NULL, comma, &end_token);
                    while(token != NULL){
                        fprintf(arq_ctl, ",%s", token);
                        token = strtok_r(NULL, comma, &end_token);
                    }
                fprintf(arq_ctl, "\n");
                break;
                }
                // anda com o token de atributos
                token2 = strtok_r(NULL, comma, &end_token2);
            }
        }
        fscanf(arq_ctl_old, "%s", info); // pega a próxima linha do arquivo antigo
        token = strtok_r(info, comma, &end_token); // atualiza o token pra nova linha
    }

    // cria um novo arquivo dad da projeção
    FILE* arq_dad = fopen(table_dad, "wt+");

    // abre o arquivo antigo da projeção
    FILE* arq_dad_old = fopen(table_dad_old, "r");



    // itera o arquivo .dad original
    for (i=0; i < tuplas; i++){
        k = 0;
        fscanf(arq_dad_old, "%s", info); // lê a tupla
        // pega a primeira celula da tupla
        token = strtok_r(info, comma, &end_token); // reutiliza as variaveis de tokens inuteis aqui
        // começa a ler toda a tupla e escreve as colunas desejadas
        for (j = 0; j <= posicoes[n-1]; j++){
            if (posicoes[k] == j){ // se a posição que você está tratando no momento é igual à posição do token atualmente
                if (k == 0)
                    fprintf(arq_dad, "%s", token);
                else fprintf(arq_dad, ",%s", token); // se não for a primeira celula bota uma virgula
                k++;
            }
            token = strtok_r(NULL, comma, &end_token); // passa pra proxima celula da tupla

        }
        fprintf(arq_dad, "\n");

    }

    fclose(arq_ctl);
    fclose(arq_ctl_old);
    fclose(arq_dad);
    fclose(arq_dad_old);



}

void create_projeta(FILE* alg, char* table, char* atributos, char* exit_table){
    int i, n=1;
    for (i=0; atributos[i] != '\0' ; i++)
        if(atributos[i] == ',')
            n++;

    fprintf(alg, "P(%s,%d,%s,%s)\n", table, n, atributos, exit_table);

}

void junta(){

}

void leitura_alg(FILE* alg){
    fseek(alg, 0L, SEEK_SET);
    int i = 1, j;
    char op;
    char A [31];  // Relação de entrada A
    char B [31]; // Relação de entrada B
    char Z [31]; // Relação de saída Z
    char request [301];
    char N [12];
    char atributos [181];
    atributos[0] = '\0'; // inicializando com lixo?
    int n;

    while (i == 1){
        i = fscanf(alg, "%c", &op);
        // operação de projeção
        if (op == 'P') {

            // P(A, N, ATTS[], Z)
            fscanf(alg, "%c", &op); // para sumir com o '('
            fscanf(alg, "%s", request);
            printf("\nRequest = %s", request);

            // pega a relação de entrada
            for (j = 0; request[j] != ',' ; j++){
                A[strlen(A)+1] = '\0';
                A[j] = request[j];
            }

            // pega o número de atributos
            printf("\nTabela = %s", A);
            int k = 0;
            for (j++;request[j] != ',';j++, k++){
                N[strlen(N)+1] = '\0';
                N[k] = request[j];
            }

            n = atoi(N);

            printf("\nn = %d", n);

            // pega os atributos com a vírgula no final
            int y = 0; // contador de vírgulas

            for (j++, k = 0; y < n ; j++, k++){
                atributos[strlen(atributos)+1] = '\0';
                atributos[k] = request[j];
                if (request[j] == ',')
                    y++;
            }

            // tira a vírgula dos atributos
            atributos[strlen(atributos) -1 ] = '\0';

            k = 0;
            Z[0] = '\0'; // Z estava inicializando com lixo
            // pega a relação de saída
            for (j;request[j] != ')';j++, k++){
                Z[strlen(Z)+1] = '\0';
                Z[k] = request[j];
            }

            printf("\nZ = %s", Z);
            printf("\natributos = %s", atributos);

            projeta(A, n, atributos, Z);
        }
        else if (op == 'S') selection();
        else if (op == 'J') junta();
    }
}
