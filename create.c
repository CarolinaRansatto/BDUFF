#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* format(char* s) {
    if (strcmp(s, "KEY") == 0) return "chv";
    if (strcmp(s, "INTEGER") == 0) return "I";
    if (strcmp(s, "STRING") == 0) return "C";
    if (strcmp(s, "NN") == 0) return "nn";
    if (strcmp(s, "ORD") == 0) return "ord";
    return s;
}

int create(FILE* sql) {
    printf("chegou no create\n");
    char narq[31], table[31], narq_dad[31];
    fscanf(sql, " TABLE %s (", table);

    // Nomeia arquivo ctl que será preenchido aqui
    strcpy(narq, table);
    strcat(narq, ".ctl");

    // nomeia arquivo dad que será preenchido posteriormente
    strcpy(narq_dad, table);
    strcat(narq_dad, ".dad");

    // cria os arquivos? Ou só cria se não existir antes?
    FILE* arq = fopen(narq, "wt"); //verificar se o arquivo existe antes?
    FILE* arq_dad = fopen(narq_dad, "wt"); //verificar se o arquivo existe antes?
    fclose(arq_dad);

    if (!arq) {
        printf("\tErro ao criar tabela %s", table);
        return 1;
    }

    // TODO: verificar se só tem uma chv e um ord

    char s[21];
    long pos = ftell(sql);
    fscanf(sql, "%s", s);
    int i = strlen(s) - 1, n = 0, a = 0;

    while (s[i] != ';') {
        if (s[i] == ',') ++n;
        fscanf(sql, "%s", s);
        i = strlen(s) - 1;
    }
    fprintf(arq, "%i,0\n", ++n);

    fseek(sql, pos, SEEK_SET);

    while (a < n) {
        fscanf(sql, "%s", s);
        i = strlen(s) - 1;
        char end = ',';
        if (s[i] == ',') {
            s[i] = '\0';
            end = '\n';
            ++a;
        }
        else if (s[i] == ';') {
            s[i - 1] = '\0';
            end = '\n';
            ++a;
        }
        strcpy(s, format(s));
        fprintf(arq, "%s%c", s, end); //precisa trocar a ordem de chv e ord?
    }

    fclose(arq);
    return 0;
}
