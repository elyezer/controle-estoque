#include "list.h"
#include "cgi.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

void parse_data(char * query, list_t ** list, const char * delim, const char * sep)
{
    size_t position = 0, total_len = 0, i = 0;
    char * token;
    char * value;
    var_t * data;

    if (*list == NULL)
    {
        *list = list_blank_list();
    }

    if (query == NULL)
        return;

    token = strtok(query, delim);
    while (token != NULL)
    {
        value = strchr(token, '=');
        *value = '\0';
        value++;

        data = (var_t *)malloc(sizeof(var_t));
        if (!data)
            return;
        memset(data, 0, sizeof(var_t));

        data->name = strdup(token);
        data->value = url_decode(value);

        list_add(*list, data);

        token = strtok(NULL, delim);
    }
}

char * url_decode(char * str)
{
    int code, i, pos;
    int len = strlen(str);
    char * result = (char *) malloc(len + 1);

    for (i = 0, pos = 0; i < len; i++)
    {
        if (str[i] == '+')
        {
            result[pos++] = ' ';
        }
        else if (str[i] == '%')
        {
            if (sscanf(&str[i + 1], "%2x", &code) != 1) {
                code = '?';
            }
            result[pos++] = code;
            i += 2;
        }
        else
        {
            result[pos++] = str[i];
        }
    }
    result[pos] = '\0';

    return result;
}

request_t * request_empty()
{
    request_t * request = (request_t *) malloc(sizeof(request_t));
    request->method = NOT_SUPPORTED;
    request->GET = NULL;
    request->POST = NULL;
    request->COOKIES = NULL;
    request->content_length = 0;
    return request;
}

void request_process(request_t ** request)
{
    if (*request == NULL)
    {
        *request = request_empty();
    }

    if (*request != NULL)
    {
        char * method = getenv("REQUEST_METHOD");
        char * content_lenght = getenv("CONTENT_LENGTH");
        char * query_string = getenv("QUERY_STRING");
        char * cookies = getenv("HTTP_COOKIE");
        char * post_data;
        char * aux;
        unsigned int i, length, pos;

        if (query_string != NULL)
        {
            parse_data(query_string, &((*request)->GET), "&", "=");
        }

        if (strcmp(method, "GET") == 0)
        {
            (*request)->method = GET;
        }
        else if (strcmp(method, "POST") == 0)
        {
            (*request)->method = POST;

            length = content_lenght != NULL ? atoi(content_lenght) : 0;
            (*request)->content_length = length;

            post_data = (char *) malloc(length + 1);
            fread(post_data, length, 1, stdin);
            post_data[length] = '\0';

            parse_data(post_data, &((*request)->POST), "&", "=");

            free(post_data);
        }
        else
        {
            (*request)->method = NOT_SUPPORTED;
        }

        if (cookies != NULL)
        {
            length = strlen(cookies);
            aux = (char *) malloc(length + 1);
            pos = 0;

            // Remove os espaços depois do ;
            for (i = 0; i < length; i++)
            {
                if (cookies[i] == ';' && cookies[i + 1] == ' ')
                {
                    aux[pos++] = ';';
                    i += 1;
                }
                else
                {
                    aux[pos++] = cookies[i];
                }
            }

            aux[pos] = '\0';
            cookies = aux;

            parse_data(cookies, &((*request)->COOKIES), ";", "=");

            free(cookies);
        }

    }
}

response_t * response_empty(const char * content_type)
{
    response_t * response = (response_t *) malloc(sizeof(response_t));
    response->headers = list_blank_list();
    response->body = NULL;
    response->length = 0;

    if (content_type != NULL)
    {
        response_add_header(&response, "Content-Type", content_type);
    }
    else
    {
        response_add_header(&response, "Content-Type", "text/html");
    }

    return response;
}

void response_write(response_t ** response, const char * text)
{
    if ((*response)->body == NULL)
    {
        (*response)->body = list_blank_list();
    }

    list_add((*response)->body, strdup(text));
    (*response)->length += strlen(text);
}

void response_write_template(response_t ** response, const char * path)
{
    FILE * template = fopen(path, "r");
    long size;
    char * buffer;

    if (template != NULL)
    {
        fseek(template, 0, SEEK_END);
        size = ftell(template);
        rewind(template);

        buffer = malloc(sizeof(char) * size + 1);

        if (buffer != NULL)
        {
            fread(buffer, 1, size, template);
            buffer[size] = '\0';
            response_write(response, buffer);
            free(buffer);
        }

        fclose(template);
    }
}

void response_add_header(response_t ** response, const char * name, const char * value)
{
    var_t * header = malloc(sizeof(var_t));
    header->name = strdup(name);
    header->value = strdup(value);

    if ((*response)->headers == NULL)
    {
        (*response)->headers = list_blank_list();
    }

    list_add((*response)->headers, header);
}

void response_set_cookie(response_t ** response, const char * name, const char * value, const char * max_age)
{
    char header[100];

    if (max_age != NULL)
    {
        sprintf(header, "%s=%s; path=%s; max-age=%s", name, value, "/", max_age);
    }
    else
    {
        sprintf(header, "%s=%s; path=%s", name, value, "/");
    }

    response_add_header(response, "Set-Cookie", header);
}

void response_send(response_t * response)
{
    node_t * node;
    var_t * header;
    char * text;
    char length[12];

    sprintf(length, "%u", response->length);
    response_add_header(&response, "Content-Length", length);

    if (response->headers != NULL)
    {
        node = response->headers->first;
        while (node != NULL)
        {
            header = (var_t *) node->data;
            if (header != NULL)
            {
                printf("%s: %s\n", header->name, header->value);
            }
            node = node->next;
        }
    }
    printf("\n");

    if (response->body != NULL)
    {
        node = response->body->first;
        while (node != NULL)
        {
            text = (char *) node->data;
            if (text != NULL)
            {
                printf("%s\n", text);
            }
            node = node->next;
        }
    }
}
