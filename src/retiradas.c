#include "list.h"
#include "cgi.h"
#include "colaboradores.h"
#include "itens.h"
#include "retiradas.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define FILE_PATH "retiradas.txt"

retirada_t * retirada_blank()
{
    retirada_t * retirada = (retirada_t *) malloc(sizeof(retirada_t));
    retirada->id = 0;
    retirada->quantidade = 0;
    retirada->data = NULL;
    retirada->colaborador = NULL;
    return retirada;
}

void retirada_free(retirada_t ** retirada)
{
    if ((*retirada)->data != NULL)
    {
        free((*retirada)->data);
    }

    if ((*retirada)->colaborador != NULL)
    {
        colaborador_free(&((*retirada)->colaborador));
    }

    free(*retirada);
}

void retiradas_load(list_t ** list, const char * filter)
{
    retirada_t * retirada;
    char data[500];
    unsigned int colaborador_id;
    colaborador_t * colaborador;
    list_t * colaboradores = NULL;
    unsigned int item_id;
    item_t * item;
    list_t * itens = NULL;
    node_t * node;
    FILE *f = fopen(FILE_PATH, "r");

    if ((*list) == NULL)
    {
        (*list) = list_blank_list();
    }

    if (f != NULL)
    {
        while(!feof(f))
        {
            retirada = retirada_blank();

            if (fscanf(f, "%u | %u | %s | %u | %u\n",
                &(retirada->id),
                &(retirada->quantidade),
                data,
                &(colaborador_id),
                &(item_id)) == 5)
            {
                if (colaboradores == NULL)
                {
                    colaboradores_load(&colaboradores, NULL);
                }

                if (itens == NULL)
                {
                    itens_load(&itens, NULL);
                }

                node = colaboradores->first;
                while (node != NULL)
                {
                    colaborador = (colaborador_t *) node->data;

                    if (colaborador_id == colaborador->id)
                    {
                        break;
                    }

                    // Não achou nenhum colaborador, só irá acontecer em caso de erro
                    if (node == colaboradores->last)
                    {
                        colaborador = NULL;
                        break;
                    }

                    node = node->next;
                }

                node = itens->first;
                while (node != NULL)
                {
                    item = (item_t *) node->data;

                    if (item_id == item->id)
                    {
                        break;
                    }

                    // Não achou nenhum item, só irá acontecer em caso de erro
                    if (node == itens->last)
                    {
                        item = NULL;
                        break;
                    }

                    node = node->next;
                }

                if (filter == NULL ||
                    (strstr(data, filter) != NULL ||
                        strstr(colaborador->nome, filter) != NULL ||
                        strstr(item->nome, filter) != NULL ||
                        retirada->id == atoi(filter) ||
                        retirada->quantidade == atoi(filter)))
                {
                    retirada->data = strdup(data);

                    if (colaborador != NULL)
                    {
                        retirada->colaborador = colaborador_blank();
                        retirada->colaborador->id = colaborador->id;
                        retirada->colaborador->matricula = colaborador->matricula;
                        retirada->colaborador->tipo = colaborador->tipo;
                        retirada->colaborador->nome = strdup(colaborador->nome);
                        retirada->colaborador->telefone = strdup(colaborador->telefone);
                        retirada->colaborador->email = strdup(colaborador->email);
                        retirada->colaborador->password = strdup(colaborador->password);
                    }

                    if (item != NULL)
                    {
                        retirada->item = item_blank();
                        retirada->item->id = item->id;
                        retirada->item->nome = strdup(item->nome);
                        retirada->item->tipo = strdup(item->tipo);
                        retirada->item->descricao = strdup(item->descricao);
                        retirada->item->quantidade = item->quantidade;
                        retirada->item->fim_estoque = strdup(item->fim_estoque);
                    }

                    list_add(*list, retirada);
                }
                else
                {
                    retirada_free(&retirada);
                }
            }
            else
            {
                retirada_free(&retirada);
            }
        }

        if (colaboradores != NULL)
        {
            node = colaboradores->first;
            while (node != NULL)
            {
                colaborador = (colaborador_t *) node->data;
                colaborador_free(&colaborador);
                node = node->next;
            }
            list_free(colaboradores);
        }

        if (itens != NULL)
        {
            node = itens->first;
            while (node != NULL)
            {
                item = (item_t *) node->data;
                item_free(&item);
                node = node->next;
            }
            list_free(itens);
        }

        fclose(f);
    }
}

void retiradas_save(list_t * list)
{
    node_t * node;
    retirada_t * retirada;
    FILE *f = fopen(FILE_PATH, "w");

    if (f != NULL)
    {
        for (node = list->first; node != NULL; node = node->next)
        {
            retirada = (retirada_t *) node->data;

            fprintf(f, "%u | %u | %s | %u | %u\n",
                retirada->id,
                retirada->quantidade,
                retirada->data,
                retirada->colaborador->id,
                retirada->item->id);
        }
    }
}
