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

void itens_load(list_t **, const char *);
void itens_save(list_t *);

#endif