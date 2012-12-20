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

    if (strcmp(request_method, "POST") == 0)
    {
        aux = getenv("CONTENT_LENGTH");
        if (aux != NULL)
            content_length = atoi(aux);

        post_data = (char *)malloc(content_length + 1);

        fread(post_data, content_length, 1, stdin);
        post_data[content_length] = '\0';

        list = list_blank_list();
        parse_data(post_data, &list, "&", "=");

        node = list->first;
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
        printf("Content-Type: text/html\n");
        printf("Set-Cookie: cgisession=%ld; path=/;\n\n", time(NULL));
        printf("Login realizado. Clique <a href=\"/mini_httpd_conf.html\">aqui</a> para configurar o servidor mini_httpd.");
        }
        else
        {
            printf("Content-Type: text/html\n\n");
            printf("Nao foi possivel realizar o login. Clique <a href=\"/\">aqui</a> para tentar novamente.");
        }
    }
    else
    {
        printf("Content-Type: text/html\n\n");
        printf("Para fazer login clique <a href=\"/\">aqui</a>.");
    }

    return 0;
}
