(function ($) {
    "use strict";

    Drupal.behaviors.hsTheme = {
        attach: function (context, settings) {
            if (settings.hasOwnProperty('hsDebugGrid') && settings.hsDebugGrid > 0
                && $('body').hasClass('toolbar')) {
                this.debugGrid(settings.hsDebugGrid);
            }
        },
        debugGrid: function (columns) {
            columns = columns || 12;

            var el = $('<div class="debug-row" />').html((function (len) {
                var html = '', i = 0;
                for (; i < len; i += 1) {
                    html += '<div class="small-1 columns"></div>';
                }

                return html;
            })(columns));

            var wrapper = jQuery('<div id="debugGrid" class="hide" />');
            wrapper.html(el);

            var toggle = $('<div class="debug-row-trigger" data-alternate="Grid overlay off" />').html('Grid overlay on');
            toggle.bind('click', function () {
                var t = this.innerHTML;

                this.innerHTML = this.getAttribute('data-alternate');
                this.setAttribute('data-alternate', t);

                wrapper.toggleClass('hide');
            });

            $('body')
                .append(wrapper)
                .append(toggle);
        }
    }
}) (jQuery);