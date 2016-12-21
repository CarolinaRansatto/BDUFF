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
    char table[101], data[101], table2[101], op[31], atributos[201], atributos2[201], exit_table[101];

    // "SELECT A,B,C FROM TABLE_NAME"
    fscanf(sql,"%s", atributos); // pega todos os atributos
    fscanf(sql,"%s", table); // "pular" o from
    fscanf(sql,"%s", table); // pega o nome da tabela

    FILE* arq_alg;

    // entrou em uma junção
    if (table[0] == '('){
        int i = 0;
        // tira o parenteses
        for (i; i < strlen(table); i++)
            table[i] = table[i+1];

        strcpy(op, table); // op recebe table
        strcat(op, ".alg"); // concatena .dad no data

        arq_alg = fopen(op, "wt+");

        fscanf(sql,"%s", data); // pega o JOIN (data é usada como auxiliar para lixo)
        fscanf(sql,"%s", table2); // pega o nome da segunda tabela

        fscanf(sql,"%s", data); // pega o ON (data é usada como auxiliar para lixo)
        fscanf(sql,"%s", atributos2); // pega a condição

        atributos2[strlen(atributos2) -2 ] = '\0'; // tira o ponto e vírugla e parenteses
        // TODO: consertar essa parte para ceitar clausulas WHERE
        create_junta(arq_alg, table, table2, atributos2, exit_table);
    }else {
        table[strlen(table)-1] = '\0'; // tira o ponto e vírgula caso n entre em uma junção
        strcpy(exit_table, table);

        strcpy(op, table); // op recebe table
        strcat(op, ".alg"); // concatena .dad no data

        arq_alg = fopen(op, "wt+");
    }

    /** WHERE -> STATMENTS
    fscanf(sql, "%s", data);
    fscanf(sql, "%s", data);

    // aqui já tem que receber as condições depois da junção
    //strcpy(table, exit_table);

    // S(A,T, O, v, Z)
    create_selection(arq_alg, table, data, exit_table);**/

    strcpy(table, exit_table);
    strcpy(data, exit_table); // data recebe table

    strcat(table, ".ctl"); // concatena .ctl no table
    strcat(data, ".dad"); // concatena .dad no data

    //TODO: abrir arquivos, achar dados, mostrar tuplas, gerar .alg
    FILE* arq_ctl = fopen(table, "rt");
    FILE* arq_dad = fopen(data, "rt");

    // tira o .ctl de table antes de mandar pro create_projeta
    table[strlen(table) - 4] = '\0';

    // junta -> seleciona -> projeta
    create_projeta(arq_alg, table, atributos, table);

    fclose(arq_dad);
    fclose(arq_ctl);

    leitura_alg(arq_alg);

    //fclose(arq_alg);
    return 0;
}

void mostraRelacao(FILE* arq_dad, FILE* arq_ctl ,char table){

    // TODO: concluir
    printf("\n\t\t|||||||| %s |||||||||\n", table);
    printf("-/----/---/----/----/---/----/----/----/---/---\n");

    fseek(arq_dad, 0L, SEEK_SET);
    fseek(arq_ctl, 0L, SEEK_SET);


}
