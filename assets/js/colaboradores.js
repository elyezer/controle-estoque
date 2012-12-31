(function ($) {
    $(function () {
        var $form = $('#form'),
            $id = $form.find('#id'),
            $matricula = $form.find('#matricula'),
            $nome = $form.find('#nome'),
            $telefone = $form.find('#telefone'),
            $email = $form.find('#email'),
            $password = $form.find('#password'),
            $tipo = $form.find('#tipo'),
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
            $matricula.val($row.find('.matricula').html());
            $nome.val($row.find('.nome').html());
            $telefone.val($row.find('.telefone').html());
            $email.val($row.find('.email').html());
            $password.val($this.data('password'));
            $tipo.val($this.data('tipo'));
            $id.val($this.data('id'));
            $form.modal();
        });

        var validator = $form.find('form').validate({
            rules: {
                matricula: {
                    required: true,
                    minlength: 1,
                    maxlength: 4,
                    digits: true,
                },
                nome: 'required',
                telefone: {
                    required: true,
                    digits: true,
                },
                email: {
                    required: true,
                    email: true,
                },
                password: 'required',
            }
        });

        $form.on('hide', function () {
            $matricula.val('');
            $nome.val('');
            $telefone.val('');
            $email.val('');
            $password.val('');
            $tipo.val('');
            $id.val('');
            validator.resetForm();
            $form.find('.error').removeClass('error');
        });
    });
})(jQuery);