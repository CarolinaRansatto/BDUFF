#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int run_sql(char* fname);

int main(int argc, char** argv) {
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
    char comando[31];

    int count = 1, r = fscanf(arq, "%s", comando);
    while (r == 1) {
        if (strcmp(comando, "CREATE") == 0)
            r = create(arq);
        else if (strcmp(comando, "INSERT") == 0)
            r = insert(arq);
        else if (strcmp(comando, "SELECT") == 0)
            r = select(arq);
        else r = 0;

        if (r) printf(" (comando %i)\n", count);
        else ++count;
        r = fscanf(arq, "%s", comando);
    }

    fclose(arq);
    return 0;
}
