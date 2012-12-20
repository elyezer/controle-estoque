#include "list.h"
#include "cgi.h"

typedef struct colaborador {
    unsigned short matricula;
    unsigned char tipo;
    char nome[50];
    char telefone[50];
    char email[50];
} colaborador_t;

int main(int argc, char const *argv[])
{
    list_t * list;

    return 0;
}