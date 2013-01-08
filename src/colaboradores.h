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

extern colaborador_t * colaborador_blank();
extern void colaborador_free(colaborador_t ** colaborador);
extern void colaboradores_load(list_t **, const char *);
extern void colaboradores_save(list_t *);

#endif
