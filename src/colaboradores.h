#ifndef COLABORADORES_H
#define COLABORADORES_H

typedef struct colaborador {
    unsigned int id;
    unsigned short matricula;
    unsigned char tipo;
    char * nome;
    char * telefone;
    char * email;
    char * password;
} colaborador_t;

void colaboradores_load(list_t **, const char *);
void colaboradores_save(list_t *);

#endif