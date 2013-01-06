#ifndef ERROR_H
#define ERROR_H

typedef enum {
    ERROR_NULL,
    ERROR_LOGIN,
    ERROR_LOGIN_REQUIRED,
    ERROR_COLABORADOR,
    ERROR_ITEM,
    ERROR_RETIRADA,
    ERROR_RETIRADA_QUANTIDADE,
    ERROR_RELATORIO_INVALIDO,
} error_t;

extern void error_page(response_t **, error_t, const char *);

#endif