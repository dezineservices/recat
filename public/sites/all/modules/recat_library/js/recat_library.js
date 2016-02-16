(function ($) {
    Drupal.behaviors.recatLibrary = {
        attach: function (context) {
            var apps = context.querySelectorAll(this.ELEMENT_SELECTOR);
            if (!apps.length) {
                return;
            }

            var i = 0,
                len = apps.length;

            for (; i < len; i += 1) {
                angular.bootstrap(apps[i], [apps[i].getAttribute('data-recat-app')]);
            }
        },
        ELEMENT_SELECTOR: '[data-recat-app]'
    };
}) (jQuery);