#include "list.h"
#include "cgi.h"
#include "itens.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define FILE_PATH "itens.txt"

item_t * item_blank()
{
    item_t * item = (item_t *) malloc(sizeof(item_t));
    item->id = 0;
    item->nome = NULL;
    item->tipo = NULL;
    item->descricao = NULL;
    item->quantidade = 0;
    item->fim_estoque = NULL;
    return item;
}

void item_free(item_t ** item)
{
    if ((*item)->nome != NULL)
    {
        free((*item)->nome);
    }

    if ((*item)->tipo != NULL)
    {
        free((*item)->tipo);
    }

    if ((*item)->descricao != NULL)
    {
        free((*item)->descricao);
    }

    if ((*item)->fim_estoque != NULL)
    {
        free((*item)->fim_estoque);
    }

    free(*item);
}

void itens_load(list_t ** list, const char * filter)
{
    item_t * item;
    char nome[500];
    char tipo[500];
    char descricao[500];
    char fim_estoque[500];
    FILE *f = fopen(FILE_PATH, "r");

    if ((*list) == NULL)
    {
        (*list) = list_blank_list();
    }

    if (f != NULL)
    {
        while(!feof(f))
        {
            item = item_blank();

            if (fscanf(f, "%u | %s | %s | %s | %u | %s\n",
                &(item->id),
                nome,
                tipo,
                descricao,
                &(item->quantidade),
                fim_estoque) == 6)
            {
                if (filter == NULL ||
                    (strstr(nome, filter) != NULL ||
                        strstr(tipo, filter) != NULL ||
                        strstr(descricao, filter) != NULL) ||
                        strstr(fim_estoque, filter) != NULL ||
                        item->id == atoi(filter) ||
                        item->quantidade == atoi(filter))
                {
                    item->nome = strdup(nome);
                    item->tipo = strdup(tipo);
                    item->descricao = strdup(descricao);
                    item->fim_estoque = strdup(fim_estoque);
                    list_add(*list, item);
                }
                else
                {
                    item_free(&item);
                }
            }
            else
            {
                item_free(&item);
            }
        }

        fclose(f);
    }
}

void itens_save(list_t * list)
{
    node_t * node;
    item_t * item;
    FILE *f = fopen(FILE_PATH, "w");

    if (f != NULL)
    {
        for (node = list->first; node != NULL; node = node->next)
        {
            item = (item_t *) node->data;

            fprintf(f, "%u | %s | %s | %s | %u | %s\n",
                item->id,
                item->nome,
                item->tipo,
                item->descricao,
                item->quantidade,
                item->fim_estoque);
        }
    }
}
