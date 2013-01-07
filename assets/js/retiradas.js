(function ($) {
    $(function () {
        var $form = $('#form'),
            $item_id = $form.find('#item_id'),
            $quantidade = $form.find('#quantidade'),
            $quantidade_help = $form.find('#quantidade-help'),
            $data = $form.find('#data'),
            $search_cancel = $('#search-cancel'),
            $search_query = $('#search-query'),
            match = window.location.search.match(/q=([^&]+)/),
            query = match && decodeURIComponent(match[1].replace(/\+/g, ' ')) || '',
            itens_options = [],
            now = new Date();

        $search_query.val(query);

        if (!query.length) {
            $search_cancel.hide();
        }

        itens_options.push('<option value=""></option>');
        $.each(itens, function (i, item) {
            itens_options.push('<option value="' + item.id + '">' + item.nome + '</option>')
        });
        $item_id.html(itens_options.join());

        var validator = $form.find('form').validate({
            rules: {
                item_id: 'required',
                quantidade: {
                    required: true,
                    digits: true,
                    min: 1,
                },
            }
        });

        $item_id.on('change', function () {
            var index = $item_id.val() - 1;
            var max_quantidade;

            if (index >= 0) {
                max_quantidade = itens[index].quantidade;
                $quantidade.rules('add', {
                    max: max_quantidade,
                });
                $quantidade_help.html(max_quantidade + ' em estoque');

                if ($quantidade.val().length) {
                    validator.form();
                }
            } else {
                $quantidade.rules('remove', 'max');
                $quantidade_help.html('');
            }
        });

        $form.on('show', function () {
            $data.val(now.getDate() + '/' + (now.getMonth() + 1) + '/' + now.getFullYear());
        });

        $form.on('hide', function () {
            $item_id.val('');
            $quantidade.val('');
            $data.val('');
            $quantidade.rules('remove', 'max');
            validator.resetForm();
            $form.find('.error').removeClass('error');
        });
    });
})(jQuery);
