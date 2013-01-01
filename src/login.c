#include "list.h"
#include "cgi.h"
#include "login.h"
#include "stdio.h"
#include "string.h"

unsigned char user_level(request_t * request)
{
    node_t * node;
    var_t * var;

    if (request->COOKIES != NULL)
    {
        node = request->COOKIES->first;
        while (node != NULL)
        {
            var = (var_t *) node->data;

            if (strcmp(var->name, "cgisession") == 0)
            {
                return atoi(var->value);
            }

            node = node->next;
        }
    }

    return ANONYMOUS;
}

void refresh_session(response_t ** response, unsigned char user_level)
{
    char aux[10];
    sprintf(aux, "%hhu", user_level);
    response_set_cookie(response, "cgisession", aux, "600");
}
