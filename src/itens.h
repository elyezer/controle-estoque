#ifndef ITENS_H
#define ITENS_H

typedef struct item {
    unsigned int id;
    char * nome;
    char * tipo;
    char * descricao;
    unsigned int quantidade;
    char * fim_estoque;
} item_t;

extern item_t * item_blank();
extern void item_free(item_t **);
extern void itens_load(list_t **, const char *);
extern void itens_save(list_t *);

#endif