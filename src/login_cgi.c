#include "list.h"
#include "cgi.h"
#include "login.h"
#include "colaboradores.h"
#include "error.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main(int argc, char const *argv[])
{
    unsigned int user_id = 0;
    unsigned char user_level = ANONYMOUS;
    list_t * list = NULL;
    node_t * node = NULL;
    var_t * data = NULL;
    colaborador_t * colaborador;
    char username[9];
    char password[9];
    request_t * request = NULL;
    response_t * response = response_empty(NULL);
    unsigned char login_done = FALSE;

    request_process(&request);

    if (request->method == POST)
    {
        node = request->POST->first;
        while (node != NULL)
        {
            data = (var_t *) node->data;
            if (strcmp(data->name, "username") == 0)
            {
                strncpy(username, data->value, 8);
                username[8] = '\0';
            }

            if (strcmp(data->name, "password") == 0)
            {
                strncpy(password, data->value, 8);
                password[8] = '\0';
            }

            node = node->next;
        }

        if (strcmp(username, USERNAME) == 0 && strcmp(password, PASSWORD) == 0)
        {
            user_id = 0;
            user_level = SUPERADMIN;
            login_done = TRUE;
        }
        else
        {
            colaboradores_load(&list, NULL);

            if (list->first != NULL)
            {
                node = list->first;
                while (node != NULL)
                {
                    colaborador = (colaborador_t *) node->data;

                    if (!login_done && colaborador->matricula == atoi(username) &&
                        strcmp(colaborador->password, password) == 0)
                    {
                        user_id = colaborador->id;
                        user_level = colaborador->tipo;
                        login_done = TRUE;
                    }

                    colaborador_free(&colaborador);

                    node = node->next;
                }

                list_free(list);
            }
        }

        if (login_done)
        {
            login_refresh_session(&response, user_id, user_level);
            response_write_template(&response, "templates/header.html");
            response_write(&response, "Login realizado. Utilize o menu para realizar as operações.");
            response_write_template(&response, "templates/footer.html");
        }
        else
        {
            error_page(&response, ERROR_LOGIN, "/");
        }
    }
    else
    {
        response_write_template(&response, "../index.html");
    }

    response_send(response);

    request_free(request);

    return 0;
}
