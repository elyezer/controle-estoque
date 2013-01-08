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

void process_report(list_t ** list, list_t * retiradas, unsigned int colaborador_id)
{
    unsigned int achou;
    node_t * retirada_node;
    node_t * node;
    retirada_t * retirada;
    retirada_t * retirada_aux;

    if ((*list) == NULL)
    {
        (*list) = list_blank_list();
    }

    retirada_node = retiradas->first;
    while (retirada_node != NULL)
    {
        retirada = (retirada_t *) retirada_node->data;
        achou = FALSE;

        node = (*list)->first;
        while (node != NULL)
        {
            retirada_aux = (retirada_t *) node->data;

            if (retirada->item->id == retirada_aux->item->id &&
                (colaborador_id == 0 || (colaborador_id > 0 && colaborador_id == retirada->colaborador->id)))
            {
                node->data = retirada;
                achou = TRUE;
            }

            node = node->next;
        }

        if (!achou)
        {
            if (colaborador_id == 0 || (colaborador_id > 0 && colaborador_id == retirada->colaborador->id))
            {
                list_add((*list), retirada);
            }
        }

        retirada_node = retirada_node->next;
    }
}

int main(int argc, char const *argv[])
{
    list_t * colaboradores = NULL;
    list_t * retiradas = NULL;
    list_t * relatorio = list_blank_list();
    node_t * node = NULL;
    colaborador_t * colaborador = NULL;
    retirada_t * retirada = NULL;
    char buffer[500];
    unsigned int colaborador_id = 0;
    char * tipo_relatorio = NULL;
    request_t * request = NULL;
    response_t * response = response_empty(NULL);
    var_t * var = NULL;
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

            if (strcmp(var->name, "por") == 0)
            {
                tipo_relatorio = var->value;
            }

            if (strcmp(var->name, "colaboradorid") == 0)
            {
                colaborador_id = atoi(var->value);
            }

            node = node->next;
        }

        colaboradores_load(&colaboradores, NULL);
        retiradas_load(&retiradas, NULL);

        if (strcmp(tipo_relatorio, "item") == 0)
        {
            process_report(&relatorio, retiradas, 0);

            login_refresh_session(&response, user_id, user_level);
            response_write_template(&response, "templates/header.html");
            response_write_template(&response, "templates/relatorios_item.html");

            if (relatorio->first == NULL)
            {
                response_write(&response, "<p>Não existe informações para serem exibidas</p>");
            }
            else
            {
                response_write(&response, "<table class=\"table table-bordered table-striped\">"
                    "<tr><th>Item</th><th>Retirado por</th><th>Data da última retirada</th>"
                    "<th>Quantidade restante</th></tr>");
                node = relatorio->first;
                while (node != NULL)
                {
                    retirada = (retirada_t *) node->data;
                    sprintf(buffer,
                        "<tr><td>%s</td><td>%s</td><td>%s</td><td class=\"quantidade\">%u</td></tr>",
                        retirada->item->nome,
                        retirada->colaborador->nome,
                        retirada->data,
                        retirada->item->quantidade);
                    response_write(&response, buffer);
                    retirada_free(&retirada);
                    node = node->next;
                }
                response_write(&response, "</table>");
            }

            response_write(&response, "<script src=\"/js/relatorios.js\"></script>");
            response_write_template(&response, "templates/footer.html");
        }
        else if (strcmp(tipo_relatorio, "colaborador") == 0)
        {
            if (colaborador_id > 0)
            {
                process_report(&relatorio, retiradas, colaborador_id);
            }

            login_refresh_session(&response, user_id, user_level);
            response_write_template(&response, "templates/header.html");
            response_write_template(&response, "templates/relatorios_colaborador.html");

            if (colaborador_id > 0)
            {
                if (relatorio->first == NULL)
                {
                    response_write(&response, "<p>Não existe informações para serem exibidas</p>");
                }
                else
                {
                    response_write(&response, "<table class=\"table table-bordered table-striped\">"
                        "<tr><th>Item</th><th>Retirado por</th><th>Data da última retirada</th>"
                        "<th>Quantidade restante</th></tr>");
                    node = relatorio->first;
                    while (node != NULL)
                    {
                        retirada = (retirada_t *) node->data;
                        sprintf(buffer,
                            "<tr><td>%s</td><td>%s</td><td>%s</td><td class=\"quantidade\">%u</td></tr>",
                            retirada->item->nome,
                            retirada->colaborador->nome,
                            retirada->data,
                            retirada->item->quantidade);
                        response_write(&response, buffer);
                        retirada_free(&retirada);
                        node = node->next;
                    }
                    response_write(&response, "</table>");
                }
            }
            else
            {
                response_write(&response, "<p id=\"message\">Para ver o relatório selecione um colaborador acima e aperte \"Ver relatório\"</p>");
            }

            response_write(&response, "<script>var colaboradores = [");

            node = colaboradores->first;
            while (node != NULL)
            {
                colaborador = (colaborador_t *) node->data;
                sprintf(buffer,
                    "{ id: %u, nome: \"%s\" },",
                    colaborador->id,
                    colaborador->nome);
                response_write(&response, buffer);
                colaborador_free(&colaborador);
                node = node->next;
            }

            response_write(&response, "];</script>");

            response_write(&response, "<script src=\"/js/relatorios.js\"></script>");
            response_write(&response, "<script src=\"/js/relatorios_colaborador.js\"></script>");
            response_write_template(&response, "templates/footer.html");
        }
        else
        {
            error = ERROR_RELATORIO_INVALIDO;
        }

        if (error != ERROR_NULL)
        {
            if (error == ERROR_RELATORIO_INVALIDO)
            {
                sprintf(buffer, "/cgi-bin/relatorios?por=item");
            }
            else
            {
                sprintf(buffer, "/cgi-bin/relatorios?por=%s", tipo_relatorio);
            }

            error_page(&response, error, buffer);
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
