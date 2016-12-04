#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int run_sql(char* fname);
int create(FILE* sql);
int insert(FILE* sql);
int select(FILE* sql);

int main(int argc, char** argv)
{
    int i, e;
    for (i = 1; i < argc; i++) {
        printf("Lendo arquivo %s\n", argv[i]);
        e = run_sql(argv[i]);
        if (e) printf("Erro ao abrir arquivo\n");
    }
    return 0;
}

int run_sql(char* fname) {
    FILE* arq = fopen(fname, "rt");
    if (!arq) return 1;
    char comando[7];
    int linha = 1, r = fscanf(arq, "%s", comando);
    while (r == 1) {
        char table[31];
        if (!strcmp(comando, "CREATE"))
            r = create(arq);
        else if (!strcmp(comando, "INSERT"))
            r = insert(arq);
        else if (!strcmp(comando, "SELECT"))
            r = select(arq);
        if (r) printf(" (linha %i)\n", linha);
        r = fscanf(arq, "%s", comando);
        ++linha;
    }
    fclose(arq);
    return 0;
}

char* format(char* s) {
    if (!strcmp(s, "KEY")) return "chv";
    if (!strcmp(s, "INTEGER")) return "I";
    if (!strcmp(s, "STRING")) return "C";
    if (!strcmp(s, "NN")) return "nn";
    if (!strcmp(s, "ORD")) return "ord";
    return s;
}

int create(FILE* sql) {
    char narq[31], table[31];
    fscanf(sql, " TABLE %s (", table);
    strcpy(narq, table);
    strcat(narq, ".ctl");
    FILE* arq = fopen(narq, "wt"); //verificar se o arquivo existe antes?
    if (!arq) {
        printf("Erro ao criar tabela %s", table);
        return 1;
    }

    char s[21];
    long pos = ftell(sql);
    fscanf(sql, "%s", s);
    int i = strlen(s) - 1, n = 0, a = 0;
    while (s[i] != ')') {
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
        if ((s[i] == ',') || s[i] == ')') {
            s[i] = '\0';
            end = '\n';
            ++a;
        }
        strcpy(s, format(s));
        fprintf(arq, "%s%c", s, end); //precisa trocar a ordem de chv e ord?
    }

    fclose(arq);
    return 0;
}

int insert(FILE* sql) {
    char table[31], data[31];
    //TODO: ler nome da tabela
    strcpy(data, table);
    strcat(table, ".ctl");
    strcat(data, ".dad");
    //TODO: abrir arquivos, verificar chave, inserir
    return 0;
}

int select(FILE* sql) {
    char table[31], data[31];
    //TODO: ler nome da tabela
    strcpy(data, table);
    strcat(table, ".ctl");
    strcat(data, ".dad");
    //TODO: abrir arquivos, achar dados, mostrar tuplas
    return 0;
}
