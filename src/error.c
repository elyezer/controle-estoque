#include "list.h"
#include "cgi.h"
#include "error.h"
#include "stdio.h"

char * error_message(error_t number)
{
    switch (number)
    {
        case ERROR_LOGIN:
            return "Nao foi possivel realizar o login";

        case ERROR_LOGIN_REQUIRED:
            return "Realize login para visualizar essa página";

        case ERROR_COLABORADOR:
            return "Nao foi possivel adicionar ou atualizar o colaborador, tente novamente mais tarde";

        case ERROR_ITEM:
            return "Nao foi possivel adicionar ou atualizar o item, tente novamente mais tarde";

        case ERROR_RETIRADA:
            return "Nao foi possivel adicionar a retirada, tente novamente mais tarde";

        case ERROR_RETIRADA_QUANTIDADE:
            return "Nao foi possivel adicionar a retirada, a quantidade informada é maior que a quantidade em estoque";

        case ERROR_RELATORIO_INVALIDO:
            return "Relatório solicitado não está disponível";

        default:
            return "";
    }
}

void error_page(response_t ** response, error_t number, const char * url)
{
    char error_number[10];

    sprintf(error_number, "%u", number);

    response_write_template(response, "templates/error_header.html");
    response_write(response, "<div class=\"alert alert-error alert-block\">"
        "<h4>Erro #");
    response_write(response, error_number);
    response_write(response, "</h4>");
    response_write(response, error_message(number));
    response_write(response, ". Clique <a href=\"");
    response_write(response, url);
    response_write(response, "\">aqui</a> para voltar."
        "</div>");
    response_write_template(response, "templates/footer.html");
}
