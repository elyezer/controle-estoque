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
        data->value = unencode(value);

        list_add(*list, data);

        token = strtok(NULL, delim);
    }
}

char * unencode(char * str)
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

request_t * request_process(int argc, char const *argv[])
{
    request_t *request = (request_t *) calloc(1, sizeof(request_t));

    if (request != NULL)
    {
        char *method = getenv("REQUEST_METHOD");
        char *content_lenght = getenv("CONTENT_LENGTH");

        request->lenght = content_lenght != NULL ? atoi(content_lenght) : 0;

        // TODO fazer o parser da QUERY_STRING
        // request->query_string = getenv("QUERY_STRING");

        if (strcmp(method, "GET") == 0)
        {
            request->method = GET;
        }
        else if (strcmp(method, "POST") == 0)
        {
            request->method = POST;
        }
        else
        {
            request->method = NOT_SUPPORTED;
        }
    }

    return request;
}


response_t *response_empty()
{
    response_t * response = malloc(sizeof(response_t));
    response->headers = NULL;
    response->body = NULL;
    response->length = 0;
    return response;
}

void response_write(response_t *res, const char *text)
{
    // text_segment *segment = malloc(sizeof(text_segment));
    // segment->text = strdup(text);
    // segment->next = NULL;
    // if (res->segment_head == NULL) {
    //     res->segment_head = segment;
    // } else {
    //     res->segment_tail->next = segment;
    // }
    // res->segment_tail = segment;
}

void response_add_header(response_t *res, const char *name, const char *val)
{
    // header *h = malloc(sizeof(header));
    // h->name = strdup(name);
    // h->value = strdup(val);
    // h->next = NULL;
    // if (res->header_head == NULL) {
    //     res->header_head = h;
    // } else {
    //     res->header_tail->next = h;
    // }
    // res->header_tail = h;
}

void response_send(response_t *res)
{
    // header *cur_h;
    // for (cur_h = res->header_head; cur_h != NULL;) {
    //     printf("%s: %s\n", cur_h->name, cur_h->value);
    //     free(cur_h->name);
    //     free(cur_h->value);
        
    //     header *next = cur_h->next;
    //     free(cur_h);
    //     cur_h = next;
    // }
    // printf("\n");
    // text_segment *cur_s;
    // for (cur_s = res->segment_head; cur_s != NULL;) {
    //     printf("%s", cur_s->text);
    //     free(cur_s->text);
        
    //     text_segment *next = cur_s->next;
    //     free(cur_s);
    //     cur_s = next;
    // }
    // free(res);
}
