#include "list.h"
#include "cgi.h"
#include "itens.h"
#include "error.h"
#include "login.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

unsigned int last_id = 0;

item_t * form_process(request_t * request)
{
    unsigned char id = 0;
    char * nome = NULL;
    char * tipo = NULL;
    char * descricao = NULL;
    char * fim_estoque = NULL;
    unsigned char quantidade = 0;
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
            nome = var->value;
        }

        if (strcmp(var->name, "tipo") == 0)
        {
            tipo = var->value;
        }

        if (strcmp(var->name, "descricao") == 0)
        {
            descricao = var->value;
        }

        if (strcmp(var->name, "fimestoque") == 0)
        {
            fim_estoque = var->value;
        }

        if (strcmp(var->name, "quantidade") == 0)
        {
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
    list_t * list = NULL;
    node_t * node = NULL;
    item_t * item = NULL;
    item_t * novo_item = NULL;
    char buffer[500];
    request_t * request = NULL;
    response_t * response = response_empty(NULL);
    var_t * var = NULL;
    char * query = NULL;
    error_t error = ERROR_NULL;
    unsigned int user_id;
    unsigned char user_level;

    request_process(&request);

    login_info(request, &user_id, &user_level);

    if (user_level > ANONYMOUS)
    {
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

        if (list->last != NULL)
        {
            item = (item_t *) list->last->data;
            last_id = item->id;
        }

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
            else
            {
                error = ERROR_ITEM;
            }
        }

        if (error != ERROR_NULL)
        {
            error_page(&response, error, "/cgi-bin/itens");
        }
        else
        {
            login_refresh_session(&response, user_id, user_level);
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
                    "<th>Quantidade</th><th>Previsão de fim de estoque</th>"
                    "<th class=\"edit\">Ações</th></tr>");
                node = list->first;
                while (node != NULL)
                {
                    item = (item_t *) node->data;
                    sprintf(buffer,
                        "<tr><td>%u</td><td class=\"nome\">%s</td><td class=\"tipo\">%s</td>"
                        "<td class=\"descricao\">%s</td><td class=\"quantidade\">%u</td>"
                        "<td class=\"fimestoque\">%s</td><td class=\"edit\">"
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
                    item_free(&item);
                    node = node->next;
                }
                response_write(&response, "</table>");
            }

            response_write(&response, "<script src=\"/js/itens.js\"></script>");
            response_write_template(&response, "templates/footer.html");
        }
    }
    else
    {
        error_page(&response, ERROR_LOGIN_REQUIRED, "/");
    }

    response_send(response);

    request_free(request);

    return 0;
}
