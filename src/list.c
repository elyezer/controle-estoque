#include "list.h"
#include "stdlib.h"

list_t * list_blank_list()
{
    list_t * list = malloc(sizeof(list_t));
    list->first = NULL;
    list->last = NULL;

    return list;
}

node_t * list_blank_node()
{
    node_t * node = malloc(sizeof(node_t));
    node->data = NULL;
    node->next = NULL;

    return node;
}

void list_add(list_t * list, void * data)
{
    node_t * node = list_blank_node();
    node->data = data;

    if (list->first == NULL) {
        list->first = node;
    } else {
        list->last->next = node;
    }
    list->last = node;
}

void list_delete(list_t * list, void * data)
{
    node_t * current_node;
    node_t * previous_node;

    for (previous_node = NULL, current_node = list->first;
         current_node != NULL;
         previous_node = current_node, current_node = current_node->next)
    {
        if (current_node->data == data)
        {
            if (previous_node == NULL)
            {
                list->first = current_node->next;

                if (list->first == NULL)
                {
                    list->last = NULL;
                }
            }
            else
            {
                previous_node->next = current_node->next;

                if (previous_node->next == NULL)
                {
                    list->last = previous_node;
                }
            }

            free(current_node);
        }
    }
}
