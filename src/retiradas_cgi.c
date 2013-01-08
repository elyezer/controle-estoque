#include "list.h"
#include "cgi.h"
#include "colaboradores.h"
#include "itens.h"
#include "retiradas.h"
#include "error.h"
#include "login.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

unsigned int last_id = 0;

retirada_t * form_process(request_t * request, unsigned int colaborador_id, list_t * colaboradores, list_t * itens)
{
    char * data = NULL;
    unsigned char quantidade = 0;
    colaborador_t * colaborador;
    unsigned char item_id = 0;
    item_t * item;
    node_t * node;
    var_t * var;
    retirada_t * retirada;

    node = request->POST->first;
    while (node != NULL)
    {
        var = (var_t *) node->data;

        if (strcmp(var->name, "data") == 0)
        {
            data = var->value;
        }

        if (strcmp(var->name, "quantidade") == 0)
        {
            quantidade = atoi(var->value);
        }

        if (strcmp(var->name, "item_id") == 0)
        {
            item_id = atoi(var->value);
        }

        node = node->next;
    }

    if (data != NULL)
    {
        retirada = retirada_blank();
        retirada->data = data;
        retirada->quantidade = quantidade;

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

        return retirada;
    }
    else
    {
        return NULL;
    }
}

int main(int argc, char const *argv[])
{
    list_t * list = NULL;
    list_t * colaboradores = NULL;
    list_t * itens = NULL;
    node_t * node = NULL;
    retirada_t * retirada = NULL;
    item_t * item = NULL;
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

        colaboradores_load(&colaboradores, NULL);
        itens_load(&itens, NULL);
        retiradas_load(&list, query);

        if (list->last != NULL)
        {
            retirada = (retirada_t *) list->last->data;
            last_id = retirada->id;
        }

        if (request->method == POST)
        {
            retirada = form_process(request, user_id, colaboradores, itens);
            if (retirada != NULL)
            {
                node = itens->first;
                while (node != NULL)
                {
                    item = (item_t *) node->data;

                    if (retirada->item->id == item->id)
                    {
                        if (retirada->quantidade > item->quantidade)
                        {
                            error = ERROR_RETIRADA_QUANTIDADE;
                            break;
                        }
                        else
                        {
                            item->quantidade = item->quantidade - retirada->quantidade;
                        }
                    }

                    node = node->next;
                }

                if (error == ERROR_NULL)
                {
                    retirada->id = ++last_id;
                    list_add(list, retirada);

                    retiradas_save(list);
                    itens_save(itens);
                }
            }
            else
            {
                error = ERROR_RETIRADA;
            }
        }

        if (error != ERROR_NULL)
        {
            error_page(&response, error, "/cgi-bin/retiradas");
        }
        else
        {
            login_refresh_session(&response, user_id, user_level);
            response_write_template(&response, "templates/header.html");
            response_write_template(&response, "templates/retiradas.html");

            if (list->first == NULL)
            {
                response_write(&response, "<p>Nenhuma retirada encontrada</p>");
            }
            else
            {
                response_write(&response, "<table class=\"table table-bordered table-striped\">"
                    "<tr><th>Id</th><th>Data</th><th>Colaborador</th><th>Item</th>"
                    "<th>Quantidade</th>");
                node = list->first;
                while (node != NULL)
                {
                    retirada = (retirada_t *) node->data;
                    sprintf(buffer,
                        "<tr><td>%u</td><td>%s</td><td>%s</td>"
                        "<td>%s</td><td>%u</td></tr>",
                        retirada->id,
                        retirada->data,
                        retirada->colaborador->nome,
                        retirada->item->nome,
                        retirada->quantidade);
                    response_write(&response, buffer);
                    retirada_free(&retirada);
                    node = node->next;
                }
                response_write(&response, "</table>");
            }

            response_write(&response, "<script>var itens = [");

            node = itens->first;
            while (node != NULL)
            {
                item = (item_t *) node->data;
                sprintf(buffer,
                    "{ id: %u, nome: \"%s\", quantidade: %u },",
                    item->id,
                    item->nome,
                    item->quantidade);
                response_write(&response, buffer);
                item_free(&item);
                node = node->next;
            }

            response_write(&response, "];</script>");

            response_write(&response, "<script src=\"/js/retiradas.js\"></script>");
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
