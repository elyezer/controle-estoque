(function ($) {
    $(function () {
        var $form = $('#form'),
            $colaborador_id = $form.find('#colaboradorid'),
            colaboradores_options = [],
            match = window.location.search.match(/colaboradorid=([^&]+)/),
            query = match && decodeURIComponent(match[1].replace(/\+/g, ' ')) || '';

        $.each(colaboradores, function (i, colaborador) {
            colaboradores_options.push('<option value="' + colaborador.id + '">' + colaborador.nome + '</option>');
        });
        if (colaboradores_options.length) {
            $colaborador_id.html(colaboradores_options.join());
        } else {
            $form.hide();
            $('#message').html('Não existe informações para serem exibidas');
        }

        if (query.length) {
            $colaborador_id.val(query);
        }
    });
})(jQuery);
