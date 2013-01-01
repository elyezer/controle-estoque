#include "list.h"
#include "colaboradores.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define FILE_PATH "colaboradores.txt"

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
