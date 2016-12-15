#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int select(FILE* sql) {
    /**
        um comando de consulta aos dados (SELECT – Seção 6) e,
        neste caso, um arquivo .alg
        contendo a sequência de comandos algébricos
        correspondentes, um em cada linha, deverá ser gerado e
        logo em seguida lido e executado.
    **/
    // TODO: testes
    char table[31], data[31], op[31], atributos[201];

    // "SELECT A,B,C FROM TABLE_NAME"
    fscanf(sql,"%s", atributos); // pega todos os atributos
    fscanf(sql,"%s", table); // "pular" o from

    fscanf(sql,"%s", table); // pega o nome da tabela

    table[strlen(table)-1] = '\0';


    strcpy(data, table); // data recebe table
    strcpy(op, table); // op recebe table
    strcat(table, ".ctl"); // concatena .ctl no table
    strcat(data, ".dad"); // concatena .dad no data
    strcat(op, ".alg"); // concatena .dad no data

    //TODO: abrir arquivos, achar dados, mostrar tuplas, gerar .alg
    FILE* arq_ctl = fopen(table, "rt");
    FILE* arq_dad = fopen(data, "rt");
    // cria o .alg
    FILE* arq_alg = fopen(op, "wt+");

    // tira o .ctl de table antes de mandar pro create_projeta
    table[strlen(table) - 4] = '\0';

    create_projeta(arq_alg, table, atributos, table);


    fclose(arq_dad);
    fclose(arq_ctl);

    leitura_alg(arq_alg);


    fclose(arq_alg);
    return 0;
}

void mostraRelacao(FILE* arq_dad, FILE* arq_ctl ,char table){

    printf("\n\t\t|||||||| %s |||||||||\n", table);
    printf("-/----/---/----/----/---/----/----/----/---/---\n");

    fseek(arq_dad, 0L, SEEK_SET);
    fseek(arq_ctl, 0L, SEEK_SET);





}
