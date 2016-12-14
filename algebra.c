#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void selection (){

}

void projeta(){

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
    char request [601];
    char N [12];
    char atributos [181];
    int n;

    while (i == 1){
        i = fscanf(alg, "%c", &op);
        if (op == 'P') {
            // P(A, N, ATTS[], Z)
            fscanf(alg, "%c", &op); // para sumir com o '('
            fscanf(alg, "%s", request);
            printf("\nRequest = %s", request);

            for (j = 0; request[j] != ',' ; j++){
                A[strlen(A)+1] = '\0';
                A[j] = request[j];
            }

            printf("\nTabela = %s", A);
            int k = 0;
            for (j++;request[j] != ',';j++, k++){
                N[strlen(N)+1] = '\0';
                N[k] = request[j];
            }

            n = atoi(N);
            //n += strlen(N);

            int y = 0; // contador de vírgulas
            for (j++, k = 0; y < n ; j++, k++){
                atributos[strlen(atributos)+1] = '\0';
                atributos[k] = request[j];
                if (request[j] == ',') y++;
            }

            printf("\natributos = %s", atributos);
            //printf("\nAtributos = %s", atributos);

            //projeta();
        }
        else if (op == 'S') selection();
        else if (op == 'J') junta();
    }
}
