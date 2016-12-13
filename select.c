#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int select(FILE* sql) {
    /**
        um comando de consulta aos dados (SELECT � Se��o 6) e,
        neste caso, um arquivo .alg
        contendo a sequ�ncia de comandos alg�bricos
        correspondentes, um em cada linha, dever� ser gerado e
        logo em seguida lido e executado.
    **/
    // TODO: testes

    char table[31], data[31];
    // "SELECT * FROM TABLE_NAME"
    fscanf(sql, " FROM %s ", table); // talvez esse regex n�o funcione
    strcpy(data, table);
    strcat(table, ".ctl");
    strcat(data, ".dad");

    //TODO: abrir arquivos, achar dados, mostrar tuplas, gerar .alg

    FILE* arq_ctl = fopen(table, "rt");
    FILE* arq_dad = fopen(data, "rt");

    fclose(arq_dad);
    fclose(arq_ctl);


    return 0;
}
