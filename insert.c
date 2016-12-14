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

int check_unique(FILE* arq, int ind, char* value) {
    fseek(arq, 0L, SEEK_SET);
    char tupla[501];
    int r = fscanf(arq, "%s", tupla), i;

    while (r == 1) {
        char* att = strtok(tupla, ",");
        for (i = 0; i < ind; i++)
            att = strtok(NULL, ",");
        if (strcmp(att, value) == 0) return 1;
        r = fscanf(arq, "%s", tupla);
    }

    return 0;
}

long find_pos(FILE* arq, int ind, char* value) {
    fseek(arq, 0L, SEEK_SET);
    char tupla[501];
    int is_int = 0;
    if (value[0] != '\'') is_int = 1;

    while (1) {
        long pos = ftell(arq);
        int r = fscanf(arq, "%s", tupla), i;
        if (r != 1) return -1;
        char *att = strtok(tupla, ",");
        for (i = 0; i < ind; i++)
            att = strtok(NULL, ",");
        //como fazer pra int?
        if (((!is_int) && ((strcmp(att, value) > 0))) ||
            ((is_int) && (atoi(att) > atoi(value))))
            return pos;
    }
}

void rewrite_dad(char* narq, long pos, char* values) {
    char nname[31] = "new_", tupla[501];
    strcat(nname, narq);
    FILE* orig = fopen(narq, "rt");
    FILE* dest = fopen(nname, "wt");

    int r = fscanf(orig, "%s", tupla);
    while ((r == 1) && (ftell(orig) <= pos)) {
        fprintf(dest, "%s\n", tupla);
        r = fscanf(orig, "%s", tupla);
    }

    fprintf(dest, "%s\n", values);

    while (r == 1) {
        fprintf(dest, "%s\n", tupla);
        r = fscanf(orig, "%s", tupla);
    }

    fclose(orig);
    fclose(dest);
    remove(narq);
    rename(nname, narq);
}

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
        printf("\tErro ao abrir o arquivo de dados %s", ndata);
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
    long pos = -1;

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

        if ((nn || chv) && (strcmp(value, "NULO") == 0)) {
            fclose(table);
            fclose(data);
            printf("\tErro: atributo %s da tabela %s nao pode ser nulo", nome, ntable);
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
            int f = check_unique(data, a, value);
            if (f) {
                fclose(table);
                fclose(data);
                printf("\tErro: impossivel inserir %s no atributo %s (tabela %s); chave ja existe", value, nome, ntable);
                return 1;
            }
        }

        if (ord) pos = find_pos(data, a, value);

        strcpy(values, tupla);
        ++a;
        r = fscanf(table, "%s", att);
    }

    if (pos != -1) {
        fclose(data);
        rewrite_dad(ndata, pos, tupla); // escreve na ordem
    }
    else {
        fseek(data, 0L, SEEK_END);
        fprintf(data, "%s\n", tupla); // escreve no fim do arquivo
        fclose(data);
    }

    if ((card/10) < ((card + 1)/10)) {
        fclose(table);
        rewrite_ctl(ntablef);
    }
    else {
        fseek(table, 0L, SEEK_SET);
        fprintf(table, "%i,%i", grau, ++card);
        fclose(table);
    }

    return 0;
}

