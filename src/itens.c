#include "list.h"
#include "cgi.h"
#include "itens.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define FILE_PATH "itens.txt"

unsigned int last_id = 0;

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

        if ((*list)->last != NULL)
        {
            item = (item_t *) (*list)->last->data;
            last_id = item->id;
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

item_t * form_process(request_t * request)
{
    unsigned char id = 0;
    char * nome = NULL;
    char * tipo = NULL;
    char * descricao = NULL;
    char * fim_estoque = NULL;
    unsigned char quantidade = 0;
    unsigned char error = FALSE;
    node_t * node;
    var_t * var;
    item_t * item;

    node = request->POST->first;
    while (node != NULL)
    {
        var = (var_t *) node->data;

        if (strcmp(var->name, "id") == 0)
        {
            id = atoi(var->value);
        }

        if (strcmp(var->name, "nome") == 0)
        {
            debug("nome\n");
            nome = var->value;
        }

        if (strcmp(var->name, "tipo") == 0)
        {
            debug("tipo\n");
            tipo = var->value;
        }

        if (strcmp(var->name, "descricao") == 0)
        {
            debug("descricao\n");
            descricao = var->value;
        }

        if (strcmp(var->name, "fimestoque") == 0)
        {
            debug("fimestoque\n");
            fim_estoque = var->value;
        }

        if (strcmp(var->name, "quantidade") == 0)
        {
            debug("quantidade\n");
            quantidade = atoi(var->value);
        }

        node = node->next;
    }

    if ((id == 0 && nome != NULL && tipo != NULL && descricao != NULL &&
        fim_estoque != NULL) || (id > 0 && fim_estoque != NULL))
    {
        item = item_blank();
        item->id = id;
        item->nome = nome;
        item->tipo = tipo;
        item->descricao = descricao;
        item->fim_estoque = fim_estoque;
        item->quantidade = quantidade;

        return item;
    }
    else
    {
        return NULL;
    }
}

int main(int argc, char const *argv[])
{
    list_t * list;
    node_t * node;
    item_t * item;
    item_t * novo_item;
    char buffer[500];
    request_t * request = NULL;
    response_t * response = response_empty(NULL);
    var_t * var;
    char * query = NULL;
    char * message = NULL;

    request_process(&request);

    node = request->GET->first;
    while (node != NULL)
    {
        var = (var_t *) node->data;

        if (strcmp(var->name, "q") == 0)
        {
            query = var->value;
        }

        node = node->next;
    }

    itens_load(&list, query);

    if (request->method == POST)
    {
        novo_item = form_process(request);
        if (novo_item != NULL)
        {
            if (novo_item->id == 0)
            {
                novo_item->id = ++last_id;
                list_add(list, novo_item);
            }
            else
            {
                node = list->first;
                while (node != NULL)
                {
                    item = (item_t *) node->data;

                    if (item->id == novo_item->id)
                    {
                        item->quantidade = novo_item->quantidade;
                        item->fim_estoque = novo_item->fim_estoque;
                        break;
                    }

                    node = node->next;
                }
            }

            itens_save(list);
        }
    }

    response_write_template(&response, "templates/header.html");
    response_write_template(&response, "templates/itens.html");

    if (list->first == NULL)
    {
        response_write(&response, "<p>Nenhum item encontrado</p>");
    }
    else
    {
        response_write(&response, "<table class=\"table table-bordered table-striped\">"
            "<tr><th>Id</th><th>Nome</th><th>Tipo</th><th>Descrição</th>"
            "<th>Quantidade</th><th>Previsão de fim de estoque</th><th>Ações</th></tr>");
        node = list->first;
        while (node != NULL)
        {
            item = (item_t *) node->data;
            sprintf(buffer,
                "<tr><td>%u</td><td class=\"nome\">%s</td><td class=\"tipo\">%s</td>"
                "<td class=\"descricao\">%s</td><td class=\"quantidade\">%u</td>"
                "<td class=\"fimestoque\">%s</td><td>"
                "<div class=\"btn-group\">"
                "<button class=\"btn edit-btn\" data-id=\"%u\"><i class=\"icon-pencil\"></i></button>"
                "</div>"
                "</td></tr>",
                item->id,
                item->nome,
                item->tipo,
                item->descricao,
                item->quantidade,
                item->fim_estoque,
                item->id);
            response_write(&response, buffer);
            node = node->next;
        }
        response_write(&response, "</table>");
    }

    response_write(&response, "<script src=\"/js/itens.js\"></script>");
    response_write_template(&response, "templates/footer.html");

    response_send(response);

    return 0;
}