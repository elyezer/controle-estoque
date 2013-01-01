#include "list.h"
#include "cgi.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"

#define USER "admin"
#define PASSWORD "master"

int main(int argc, char const *argv[])
{
    char * request_method = getenv("REQUEST_METHOD");
    char * aux;
    int content_length = 0;
    char * post_data;
    list_t * list = NULL;
    node_t * node = NULL;
    var_t * data = NULL;
    char username[9];
    char password[9];
    request_t * request = NULL;
    response_t * response = response_empty(NULL);

    request_process(&request);

    if (strcmp(request_method, "POST") == 0)
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

        if (strcmp(username, USER) == 0 && strcmp(password, PASSWORD) == 0)
        {
            aux = malloc(100 * sizeof(char));
            sprintf(aux, "%ld", time(NULL));

            // TODO definir como será guardado o usuário logado
            response_set_cookie(&response, "cgisession", aux, "600");
            response_write_template(&response, "templates/header.html");
            response_write(&response, "Login realizado. Utilize o menu para realizar as operações.");
            response_write_template(&response, "templates/footer.html");

            free(aux);
        }
        else
        {
            response_write_template(&response, "templates/header.html");
            response_write(&response, "Nao foi possivel realizar o login. Clique <a href=\"/\">aqui</a> para tentar novamente.");
            response_write_template(&response, "templates/footer.html");
        }
    }
    else
    {
        response_write_template(&response, "index.html");
    }

    response_send(response);

    request_free(request);

    return 0;
}
