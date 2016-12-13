#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void rewrite_ctl(char* narq) {
    char nname[31] = "new_", att[501];
    strcat(nname, narq);
    FILE* orig = fopen(narq, "rt");
    FILE* dest = fopen(nname, "wt");
    int grau, card;
    fscanf(orig, "%i,%i", &grau, &card);
    fprintf(dest, "%i,%i\n", grau, card + 1);
    int r = fscanf(orig, "%s", att);
    while (r == 1) {
        fprintf(dest, "%s\n", att);
        r = fscanf(orig, "%s", att);
    }
    fclose(orig);
    fclose(dest);
    remove(narq);
    rename(nname, narq);
}

char* find_value(FILE* arq);
long find_value_pos(FILE* arq);
void rewrite_dad(char* narq);

int insert(FILE* sql) {
    char ntable[31], ntablef[31], ndata[31], tupla[501], att[501], values[501];
    fscanf(sql, "%s", ntable); //pega o INTO
    fscanf(sql, "%s", ntable); //colocar "INTO %s" não funcionou

    strcpy(ntablef, ntable);
    strcpy(ndata, ntable);
    strcat(ntablef, ".ctl");
    strcat(ndata, ".dad");

    fscanf(sql, "%s", tupla); //pega o VALUES
    fscanf(sql, "%s", tupla); //pega a tupla, "(%s);" não funciona

    FILE* table = fopen(ntablef, "rt+");
    if (!table) {
        printf("\tTentativa de inserir em tabela nao existente: %s", ntable);
        return 1;
    }

    FILE* data = fopen(ndata, "at+");
    if (!data) {
        fclose(table);
        printf("\tErro ao criar o arquivo de dados %s", ndata);
        return 1;
    }

    int i = 0;
    while (tupla[i] != '\0') {
        // tira os "(" e ");" depois traz todo o resto da string pra frente
        // Aqui é while e não if por conta do ); (devem ser retirados em sequencia)
        while (tupla[i] == '(' || tupla[i] == ')' || tupla[i]== ';') {
            int j = i;
            while (tupla[j] != '\0'){
                tupla[j] = tupla[j+1];
                j++;
            }
        }
        if (tupla[i] == '"')
            tupla[i] = '\'';
        ++i;
    }

    int grau, card;
    fscanf(table, "%i,%i", &grau, &card);

    int r = fscanf(table, "%s", att), a = 0;
    strcpy(values, tupla);
    long pos;

    while (r == 1) {
        int nn = 0, chv = 0, ord = 0, is_int = 0;
        char *ctl = strtok(att, ","), *nome = ctl;
        while (ctl) {
            if (strcmp(ctl, "nn") == 0) nn = 1;
            if (strcmp(ctl, "chv") == 0) chv = 1;
            if (strcmp(ctl, "ord") == 0) ord = 1;
            if (strcmp(ctl, "I") == 0) is_int = 1;
            ctl = strtok(NULL, ",");
        }

        char* value = strtok(values, ",");
        for (i = 0; i < a; i++)
            value = strtok(NULL, ",");
        strcpy(values, tupla);

        if ((nn) && (strcmp(value, "NULO") == 0)) {
            fclose(table);
            fclose(data);
            printf("\tErro: atributo %s da tabela %s não pode ser nulo", nome, ntable);
            return 1;
        }

        if ((is_int) && (value[0] == '\'')) {
            fclose(table);
            fclose(data);
            printf("\tErro: impossivel inserir %s no atributo %s (tabela %s); deve ser inteiro", value, nome, ntable);
            return 1;
        }

        if ((!is_int) && (value[0] != '\'')) {
            fclose(table);
            fclose(data);
            printf("\tErro: impossivel inserir %s no atributo %s (tabela %s); deve ser string", value, nome, ntable);
            return 1;
        }

        if (chv) {
            char res[31] = find_value(data, a, value);
            if (strcmp(res, value) == 0) {
                fclose(table);
                fclose(data);
                printf("\tErro: impossivel inserir %s no atributo %s (tabela %s); chave já existe", value, nome, ntable);
                return 1;
            }
        }

        if (ord) pos = find_value_pos(data, a, value);

        ++a;
        r = fscanf(table, "%s", att);
    }

    if (ord) rewrite_dad(pos, tupla);
    else fprintf(data, "%s\n", tupla); // escreve no fim do arquivo

    if ((card/10) < ((card + 1)/10)) {
        fclose(table);
        rewrite_ctl(ntablef);
    }
    else {
        fseek(table, 0L, SEEK_SET);
        fprintf(table, "%i,%i", grau, ++card);
        fclose(table);
    }

    fclose(data);
    return 0;
}

