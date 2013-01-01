#ifndef LIST_H
#define LIST_H

struct node
{
    void * data;
    struct node * next;
};
typedef struct node node_t;

typedef struct list
{
    node_t * first;
    node_t * last;
    unsigned int length;
} list_t;

list_t * list_blank_list();
void list_add(list_t * list, void * data);
void list_delete(list_t * list, void * data);
void list_free(list_t * list);

#endif
