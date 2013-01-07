(function ($) {
    $(function () {
        var $quantidades = $('.quantidade');

        $quantidades.each(function (index, element) {
            var $row = $(element);

            if ($row.html() < 10) {
                $row.parent().addClass('warning');
            }
        });
    });
})(jQuery);
