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
    char narq[31], table[31], narq_dad[31];
    fscanf(sql, "%s", table); // lê o TABLE
    fscanf(sql, "%s", table); // lê o nome da tabela

    // Nomeia arquivo ctl que será preenchido aqui
    strcpy(narq, table);
    strcat(narq, ".ctl");

    // nomeia arquivo dad que será preenchido posteriormente
    strcpy(narq_dad, table);
    strcat(narq_dad, ".dad");

    //verifica se a tabela já existe
    FILE* arq = fopen(narq, "rt");
    if (arq) {
        fclose(arq);
        printf("Erro: tabela %s ja existe", table);
        return 1;
    }
    fclose(arq);

    // cria os arquivos
    arq = fopen(narq, "wt");
    FILE* arq_dad = fopen(narq_dad, "wt");
    fclose(arq_dad);

    if (!arq) {
        fclose(arq);
        printf("Erro ao criar tabela %s", table);
        return 1;
    }

    // TODO: verificar se só tem uma chv e um ord

    char s[21];
    fscanf(sql, "%s", s); // lê o "("
    long pos = ftell(sql);

    fscanf(sql, "%s", s);
    int n = 0, p = 0, i = strlen(s) - 1;

    while (s[i] != ';') {
        if (s[i] == ',') ++n;
        fscanf(sql, "%s", s);
        i = strlen(s) - 1;
    }
    fprintf(arq, "%i,0\n", ++n);

    fseek(sql, pos, SEEK_SET);

    int chv = -1, ord = -1, nn = -1, a = 0;

    while (p < n) {
        fscanf(sql, "%s", s);
        i = strlen(s) - 1;
        char end = ',';

        if (s[i] == ',') {
            s[i] = '\0';
            end = '\n';
            ++p;
        }
        else if (s[i] == ';') {
            s[i - 1] = '\0';
            end = '\n';
            ++p;
            ++a;
        }

        strcpy(s, format(s));

        if (strcmp(s, "nn") == 0)
            nn = a;

        if (strcmp(s, "chv") == 0) {
            if (chv >= 0) {
                fclose(arq);
                fclose(arq_dad);
                remove(narq);
                remove(narq_dad);
                printf("Erro: duas chaves definidas na tabela %s; tabela nao criada", table);
                return 1;
            }
            else chv = a;

            if (nn != chv) {
                fclose(arq);
                fclose(arq_dad);
                remove(narq);
                remove(narq_dad);
                printf("Erro: tentativa de criar chave que permite nulo na tabela %s; tabela nao criada", table);
                return 1;
            }
        }

        if (strcmp(s, "ord") == 0) {
            if (ord >= 0) {
                fclose(arq);
                fclose(arq_dad);
                remove(narq);
                remove(narq_dad);
                printf("Erro: tentativa de ordenar tabela %s por dois atributos; tabela nao criada", table);
                return 1;
            }
            else ord = a;

            if (nn != chv) {
                fclose(arq);
                fclose(arq_dad);
                remove(narq);
                remove(narq_dad);
                printf("Erro: tentativa de ordernar tabela %s por atributo que permite nulo; tabela nao criada", table);
                return 1;
            }
        }

        fprintf(arq, "%s%c", s, end);
    }

    fclose(arq);
    return 0;
}
