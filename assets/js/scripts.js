(function ($) {
    $(function () {
        var match = document.cookie.match(/cgisession=\d+-([^;])/),
            user_level = match && decodeURIComponent(match[1].replace(/\+/g, ' ')) || '',
            $colaboradores = $('#nav-colaboradores'),
            $itens = $('#nav-itens'),
            $retiradas = $('#nav-retiradas'),
            $relatorios = $('#nav-relatorios'),
            $add = $('#add'),
            $edit = $('.edit');

        if (user_level.length) {
            if (user_level < 3) {
                $add.remove();
                $edit.remove();
            }

            if (user_level == 1) {
                $colaboradores.remove();
                $retiradas.remove();
                $relatorios.remove();
            }

            if (user_level == 4) {
                $itens.remove();
                $retiradas.remove();
                $relatorios.remove();
            }
        }
    });

    $.validator.addMethod('date', function (value, element) {
        return this.optional(element) || /^\d{1,2}[\/\-]\d{1,2}[\/\-]\d{4}$/.test(value);
    }, jQuery.validator.messages.date);
})(jQuery);
