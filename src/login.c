#include "list.h"
#include "cgi.h"
#include "login.h"
#include "stdio.h"
#include "string.h"

void login_info(request_t * request, unsigned int * user_id, unsigned char * user_level)
{
    node_t * node;
    var_t * var;
    *user_id = 0;
    *user_level = ANONYMOUS;

    if (request->COOKIES != NULL)
    {
        node = request->COOKIES->first;
        while (node != NULL)
        {
            var = (var_t *) node->data;

            if (strcmp(var->name, "cgisession") == 0)
            {
                sscanf(var->value, "%u-%hhu", user_id, user_level);
                break;
            }

            node = node->next;
        }
    }
}

void login_refresh_session(response_t ** response, unsigned int user_id, unsigned char user_level)
{
    char aux[10];
    sprintf(aux, "%u-%hhu", user_id, user_level);
    response_set_cookie(response, "cgisession", aux, "600");
}
