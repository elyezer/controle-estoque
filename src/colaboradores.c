#include "list.h"
#include "cgi.h"
#include "colaboradores.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define FILE_PATH "colaboradores.txt"

unsigned int last_id = 0;

colaborador_t * colaborador_blank()
{
    colaborador_t * colaborador = (colaborador_t *) malloc(sizeof(colaborador_t));
    colaborador->id = 0;
    colaborador->matricula = 0;
    colaborador->tipo = 0;
    colaborador->nome = NULL;
    colaborador->telefone = NULL;
    colaborador->email = NULL;
    colaborador->password = NULL;
    return colaborador;
}

void colaborador_free(colaborador_t ** colaborador)
{
    if ((*colaborador)->nome != NULL)
    {
        free((*colaborador)->nome);
    }

    if ((*colaborador)->telefone != NULL)
    {
        free((*colaborador)->telefone);
    }

    if ((*colaborador)->email != NULL)
    {
        free((*colaborador)->email);
    }

    if ((*colaborador)->password != NULL)
    {
        free((*colaborador)->password);
    }

    free(*colaborador);
}

void colaboradores_load(list_t ** list, const char * filter)
{
    colaborador_t * colaborador;
    char nome[500];
    char telefone[500];
    char email[500];
    char password[500];
    FILE *f = fopen(FILE_PATH, "r");

    if ((*list) == NULL)
    {
        (*list) = list_blank_list();
    }

    if (f != NULL)
    {
        while(!feof(f))
        {
            colaborador = colaborador_blank();

            if (fscanf(f, "%u | %hu | %hhu | %s | %s | %s | %s\n",
                &(colaborador->id),
                &(colaborador->matricula),
                &(colaborador->tipo),
                nome,
                telefone,
                email,
                password) == 7)
            {
                if (filter == NULL ||
                    (strstr(nome, filter) != NULL ||
                        strstr(telefone, filter) != NULL ||
                        strstr(email, filter) != NULL) ||
                        colaborador->id == atoi(filter) ||
                        colaborador->matricula == atoi(filter))
                {
                    colaborador->nome = strdup(nome);
                    colaborador->telefone = strdup(telefone);
                    colaborador->email = strdup(email);
                    colaborador->password = strdup(password);
                    list_add(*list, colaborador);
                }
                else
                {
                    colaborador_free(&colaborador);
                }
            }
            else
            {
                colaborador_free(&colaborador);
            }
        }

        if ((*list)->last != NULL)
        {
            colaborador = (colaborador_t *) (*list)->last->data;
            last_id = colaborador->id;
        }

        fclose(f);
    }
}

void colaboradores_save(list_t * list)
{
    node_t * node;
    colaborador_t * colaborador;
    FILE *f = fopen(FILE_PATH, "w");

    if (f != NULL)
    {
        for (node = list->first; node != NULL; node = node->next)
        {
            colaborador = (colaborador_t *) node->data;

            fprintf(f, "%u | %hu | %hhu | %s | %s | %s | %s\n",
                colaborador->id,
                colaborador->matricula,
                colaborador->tipo,
                colaborador->nome,
                colaborador->telefone,
                colaborador->email,
                colaborador->password);
        }
    }
}

colaborador_t * form_process(request_t * request)
{
    unsigned short matricula = 0;
    char * nome = NULL;
    char * telefone = NULL;
    char * email = NULL;
    char * password = NULL;
    unsigned char tipo = 0;
    unsigned char id = 0;
    unsigned char error = FALSE;
    node_t * node;
    var_t * var;
    colaborador_t * colaborador;

    node = request->POST->first;
    while (node != NULL)
    {
        var = (var_t *) node->data;

        if (strcmp(var->name, "matricula") == 0)
        {
            matricula = atoi(var->value);
        }

        if (strcmp(var->name, "nome") == 0)
        {
            nome = var->value;
        }

        if (strcmp(var->name, "telefone") == 0)
        {
            telefone = var->value;
        }

        if (strcmp(var->name, "email") == 0)
        {
            email = var->value;
        }

        if (strcmp(var->name, "password") == 0)
        {
            password = var->value;
        }

        if (strcmp(var->name, "tipo") == 0)
        {
            tipo = atoi(var->value);
        }

        if (strcmp(var->name, "id") == 0)
        {
            id = atoi(var->value);
        }

        node = node->next;
    }

    if (nome != NULL && telefone != NULL && email != NULL &&
        password != NULL && tipo > 0 && tipo < 4)
    {
        colaborador = colaborador_blank();
        colaborador->id = id;
        colaborador->matricula = matricula;
        colaborador->nome = nome;
        colaborador->telefone = telefone;
        colaborador->email = email;
        colaborador->password = password;
        colaborador-> tipo = tipo;

        return colaborador;
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
    colaborador_t * colaborador;
    colaborador_t * novo_colaborador;
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

    colaboradores_load(&list, query);

    if (request->method == POST)
    {
        novo_colaborador = form_process(request);
        if (novo_colaborador != NULL)
        {
            if (novo_colaborador->id == 0)
            {
                novo_colaborador->id = ++last_id;
                list_add(list, novo_colaborador);
            }
            else
            {
                node = list->first;
                while (node != NULL)
                {
                    colaborador = (colaborador_t *) node->data;

                    if (colaborador->id == novo_colaborador->id)
                    {
                        colaborador->matricula = novo_colaborador->matricula;
                        colaborador->nome = novo_colaborador->nome;
                        colaborador->telefone = novo_colaborador->telefone;
                        colaborador->email = novo_colaborador->email;
                        colaborador->password = novo_colaborador->password;
                        colaborador-> tipo = novo_colaborador->tipo;
                        break;
                    }

                    node = node->next;
                }
            }

            colaboradores_save(list);
        }
    }

    response_write_template(&response, "templates/header.html");
    response_write_template(&response, "templates/colaboradores.html");

    if (list->first == NULL)
    {
        response_write(&response, "<p>Nenhum colaborador encontrado</p>");
    }
    else
    {
        response_write(&response, "<table class=\"table table-bordered table-striped\">"
            "<tr><th>Id</th><th>Matrícula</th><th>Nome</th><th>Telefone</th><th>Email</th><th>Ações</th></tr>");
        node = list->first;
        while (node != NULL)
        {
            colaborador = (colaborador_t *) node->data;
            sprintf(buffer,
                "<tr><td>%u</td><td class=\"matricula\">%04hu</td><td class=\"nome\">%s</td>"
                "<td class=\"telefone\">%s</td><td class=\"email\">%s</td><td>"
                "<div class=\"btn-group\">"
                "<button class=\"btn edit-btn\" data-id=\"%u\" data-tipo=\"%hhu\""
                "data-password=\"%s\"><i class=\"icon-pencil\"></i></button>"
                "</div>"
                "</td></tr>",
                colaborador->id,
                colaborador->matricula,
                colaborador->nome,
                colaborador->telefone,
                colaborador->email,
                colaborador->id,
                colaborador->tipo,
                colaborador->password);
            response_write(&response, buffer);
            colaborador_free(&colaborador);
            node = node->next;
        }
        response_write(&response, "</table>");
    }

    response_write(&response, "<script src=\"/js/colaboradores.js\"></script>");
    response_write_template(&response, "templates/footer.html");

    response_send(response);

    request_free(request);

    return 0;
}