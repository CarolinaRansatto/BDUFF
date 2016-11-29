#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int run_sql(char* fname);
int create(char* table);
int insert(char* table);
int select(char* table);

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
    while (r) {
        char table[31];
        if (strcmp(comando, "CREATE")) {
            fscanf(arq, "TABLE %s(", table); //isso funciona?
            //TODO: pegar atributos. como ir até achar o ")"?
            r = create(table);
        }
        else if (strcmp(comando, "INSERT")) {
            //TODO: pegar nome da tabela e tupla
            //r = insert();
        }
        else if (strcmp(comando, "SELECT")) {
            //TODO: pegar atributos, nome da tabela e condição
            //r = select();
        }
        if (r) printf(" (linha %i)\n", linha);
        r = fscanf(arq, "%s", comando);
        ++linha;
    }
    fclose(arq);
    return 0;
}

int create(char* table) {
    //TODO: receber atributos? ou abrir arquivo?
    //TODO: avisar se deu erro (ou imprimir aqui mesmo?)
    char narq[31];
    strcpy(narq, table);
    strcat(narq, ".ctl");
    FILE* arq = fopen(narq, "wt"); //verificar se o arquivo existe antes?
    if (!arq) {
        printf("Erro ao criar tabela %s", table);
        return 1;
    }
    //TODO: escrever dados da tabela no arquivo
    fclose(arq);
    return 0;
}

int insert(char* table) {
    //char* ntable = strcat(table, ".ctl");
    //char* ndata = strcat(table, ".dad");
    return 0;
}

int select(char* table) {
    //char* ntable = strcat(table, ".ctl");
    //char* ndata = strcat(table, ".dad");
    return 0;
}
