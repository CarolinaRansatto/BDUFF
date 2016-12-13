#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int run_sql(char* fname);

int main(int argc, char** argv) {
    int i, e;
    for (i = 1; i < argc; i++) {
        printf("Lendo arquivo %s\n", argv[i]);
        e = run_sql(argv[i]);
        if (e) printf("\tErro ao abrir arquivo\n");
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
<<<<<<< HEAD

        if (r) printf(" (comando %i)\n", count);
        else ++count;
        r = fscanf(arq, "%s", comando);
=======

        if (r) printf(" (comando %i)\n", count);
        else ++count;
        r = fscanf(arq, "%s", comando);
    }

    fclose(arq);
    return 0;
}

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
        else if  (s[i] == ';') {
            s[i - 1] = '\0';
            end = '\n';
            ++a;
        }
        strcpy(s, format(s));
        fprintf(arq, "%s%c", s, end); //precisa trocar a ordem de chv e ord?
>>>>>>> 3efa8ca5ca17b5251ff3e25b4cd5110ccaba4416
    }

    fclose(arq);
    return 0;
<<<<<<< HEAD
}
=======
}

int insert(FILE* sql) {
    char ntable[31], ntablef[31], ndata[31], tupla[501], att[501];
    fscanf(sql, "%s", ntable); //pega o INTO
    fscanf(sql, "%s", ntable); //colocar "INTO %s" não funcionou

    strcpy(ntablef, ntable);
    strcpy(ndata, ntable);
    strcat(ntablef, ".ctl");
    strcat(ndata, ".dad");

    fscanf(sql, "%s", tupla); //pega o VALUES
    fscanf(sql, "%s", tupla); //pega a tupla, "(%s);" não funciona

    FILE* table = fopen(ntablef, "rt");
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
        while (tupla[i] == '(' || tupla[i] == ')' || tupla[i]== ';'){
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

    //verifica vários att
    while (r == 1) {
        printf("%i\n", a);
        int nn = 0, chv = 0, ord = 0;
        char *ctl = strtok(att, ","), *nome = ctl;
        while (ctl) {
            if (strcmp(ctl, "nn") == 0) nn = 1;
            if (strcmp(ctl, "chv") == 0) chv = 1;
            if (strcmp(ctl, "ord") == 0) ord = 1;
            ctl = strtok(NULL, ",");
        }
        printf("passou do while ctl\n");
        char* value = strtok(tupla, ",");
        for (i = 0; i < a; i++)
            value = strtok(NULL, "(,);");
        printf("passou do for\n");
        if ((nn) && (strcmp(value, "NULO") == 0)) {
            fclose(table);
            fclose(data);
            printf("\tErro: atributo %s da tabela %s não pode ser nulo", nome, ntable);
            return 1;
        }
        if (chv || ord) {
            //TODO: fazer processo chaaaaato de percorrer o arquivo de dados, verificar unicidade da chv e ordenação
            //se for ord pode ter que reescrever o arquivo
        }
        ++a;
        r = fscanf(table, "%s", att);
    }

    fprintf(data, "%s\n", tupla); // escreve no fim do arquivo

    if ((card/10) < ((card + 1)/10)) {
        fclose(table);
        //rewrite_file(ntablef);
    }
    else {
        fseek(table, 0L, SEEK_SET);
        fprintf(table, "%i,%i", grau, ++card);
        fclose(table);
    }

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


>>>>>>> 3efa8ca5ca17b5251ff3e25b4cd5110ccaba4416
