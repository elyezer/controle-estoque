(function ($) {
    $(function () {
        var $form = $('#form'),
            $id = $form.find('#id'),
            $nome = $form.find('#nome'),
            $tipo = $form.find('#tipo'),
            $descricao = $form.find('#descricao'),
            $quantidade = $form.find('#quantidade'),
            $fim_estoque = $form.find('#fim-estoque'),
            $search_cancel = $('#search-cancel'),
            $search_query = $('#search-query'),
            match = window.location.search.match(/q=([^&]+)/),
            query = match && decodeURIComponent(match[1].replace(/\+/g, ' ')) || '';

        $search_query.val(query);

        if (!query.length) {
            $search_cancel.hide();
        }

        $('button[data-id]').on('click', function () {
            var $this = $(this),
                $row = $this.parent().parent().parent();
            $nome.val($row.find('.nome').html()).prop("disabled", true);
            $tipo.val($row.find('.tipo').html()).prop("disabled", true);
            $descricao.val($row.find('.descricao').html()).prop("disabled", true);
            $quantidade.val($row.find('.quantidade').html());
            $fim_estoque.val($row.find('.fim-estoque').html());
            $id.val($this.data('id'));
            $form.modal();
        });

        var validator = $form.find('form').validate({
            rules: {
                nome: 'required',
                tipo: 'required',
                descricao: 'required',
                quantidade: {
                    required: true,
                    digits: true,
                },
                fim_estoque: {
                    required: true,
                    date: true,
                },
            }
        });

        $form.on('hide', function () {
            $nome.val('').prop("disabled", false);
            $tipo.val('').prop("disabled", false);
            $descricao.val('').prop("disabled", false);
            $quantidade.val('');
            $fim_estoque.val('');
            $id.val('');
            validator.resetForm();
            $form.find('.error').removeClass('error');
        });
    });
})(jQuery);