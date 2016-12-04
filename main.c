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
    int count = 1, r = fscanf(arq, "%s", comando);
    while (r == 1) {
        if (!strcmp(comando, "CREATE"))
            r = create(arq);
        else if (!strcmp(comando, "INSERT"))
            r = insert(arq);
        else if (!strcmp(comando, "SELECT"))
            r = select(arq);
        else r = 0;
        if (r) printf(" (comando %i)\n", count); //mudar pra linha e os métodos retornam quantas linhas andaram no arq?
        r = fscanf(arq, "%s", comando);
        ++count; //conta errado se um comando der erro
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
    char narq[31], table[31], narqdad[31];
    fscanf(sql, " TABLE %s (", table);

    // Nomeia arquivo ctl que será preenchido aqui
    strcpy(narq, table);
    strcat(narq, ".ctl");

    // nomeia arquivo dad que será reenchido posteriormente
    strcpy(narq_dad, table);
    strcat(narq_dad, ".dad");

    // cria os arquivos? Ou só cria se não existir antes?
    FILE* arq = fopen(narq, "wt"); //verificar se o arquivo existe antes?
    FILE* arqdad = fopen(narqdad, "wt"); //verificar se o arquivo existe antes?
    fclose(arq_dad);

    if (!arq) {
        printf("Erro ao criar tabela %s", table);
        return 1;
    }

    char s[21];
    long pos = ftell(sql);
    fscanf(sql, "%s", s);
    int i = strlen(s) - 1, n = 0, a = 0;

    while (s[i] != ')') { //na verdade o fim deveria ser );
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
    char ntable[31], ntablef[31], ndata[31], tupla[501];
    fscanf(sql, "%s", ntable); //pega o INTO
    ntable = fscanf(sql, "%s", ntable); //colocar "INTO %s" não funcionou

    strcpy(ntablef, ntable);
    strcpy(ndata, ntable);
    strcat(ntablef, ".ctl");
    strcat(ndata, ".dad");

    fscanf(sql, "%s", tupla); //pega o VALUES
    fscanf(sql, "%s", tupla); //pega a tupla, "(%s);" não funciona

    FILE* table = fopen(ntablef, "rt");
    if (!table) {
        printf("Tentativa de inserir em tabela nao existente: %s", ntable);
        return 1;
    }
    fclose(table);
    FILE* data = fopen(ndata, "at+");
    if (!data) {
        printf("Erro ao criar o arquivo de dados %s", ndata);
        fclose(table);
        return 1;
    }

    int i = 0;
    while (tupla[i] != '\0') {
        // tira os "(" e ");" depois traz todo o resto da string pra frente
        if (tupla[i] == '(' || tupla[i] == ')' || tupla[i]== ';'){
            int j = i;
            while (tupla[j] != '\0'){
                tupla[j] = tupla[j+1];
                j++;
            }
            tupla[j] = NULL;
        }

        if (tupla[i] == '"')
            tupla[i] = '\'';
        ++i;
    }
    //TODO: verificar chave, implementar chave ordenada, verificar nulo
    fprintf(data, "%s\n", tupla);
    fclose(data);
    return 0;
}

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
    table = fscanf(sql, " FROM %s ", table); // talvez esse regex não funcione
    strcpy(data, table);
    strcat(table, ".ctl");
    strcat(data, ".dad");

    //TODO: abrir arquivos, achar dados, mostrar tuplas, gerar .alg

    FILE* arq_ctl = fopen(fname, "rt");
    FILE* arq_dad = fopen(fname, "rt");

    fclose(arq_dad);
    fclose(arq_ctl);


    return 0;
}
