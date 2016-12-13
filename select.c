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

    char table[31], data[31];
    // "SELECT * FROM TABLE_NAME"
    fscanf(sql, " FROM %s ", table); // talvez esse regex não funcione
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
