#ifndef RETIRADAS_H
#define RETIRADAS_H

typedef struct retirada {
    unsigned int id;
    unsigned int quantidade;
    char * data;
    colaborador_t * colaborador;
    item_t * item;
} retirada_t;

extern retirada_t * retirada_blank();
extern void retirada_free(retirada_t **);
extern void retiradas_load(list_t **, const char *);
extern void retiradas_save(list_t *);

#endif
