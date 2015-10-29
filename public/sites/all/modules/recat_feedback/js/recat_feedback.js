(function ($) {
    Drupal.behaviors.recatFeedback = {
        attach: function (context) {
            var el = $(this.ELEMENT_SELECTOR, context);

            for (var i = 0, len = el.length; i < len; i += 1) {
                this.ratingStar($(el[i]));
            }
        },
        ratingStar: function (el) {
            el.addClass('form-radios-ratingstars');

            $('input[type="radio"]', el).rating({
                required: false
            }).parent().first().addClass('form-radios-ratingstar');
        },
        ELEMENT_SELECTOR: '.js-ratingstars'
    };
}) (jQuery);