(function ($) {
    Drupal.behaviors.recatWfOverlay = {
        attach: function (context) {
            this.detach(context);
            
            $(this.ELEMENT_SELECTOR, context).bind(this.CLICK_EVENT, this.onClick);
            $(this.ELEMENT_CLOSE_SELECTOR, context).bind(this.CLICK_EVENT, this.onCloseClick);

            this.resizeOverlayer();
        },
        detach : function (context) {
            $(this.ELEMENT_SELECTOR, context).unbind(this.CLICK_EVENT)
            $(this.ELEMENT_CLOSE_SELECTOR, context).unbind(this.CLICK_EVENT)
        },
        onClick: function () {
            var href = this.getAttribute('data-href-overlay');
            Drupal.behaviors.recatWfOverlay.openOverlayer(href || this.href);
            return false;
        },
        onCloseClick: function () {
            window.parent.jQuery.magnificPopup.close();
            return false;
        },
        openOverlayer: function (url) {
            $.magnificPopup.open({
                items: {
                    src: url
                },
                iframe: {
                    markup: '<div class="webform-overlay mfp-iframe-scaler">' +
                    '<iframe class="mfp-iframe" frameborder="0" allowfullscreen></iframe>' +
                    '</div>'
                },
                modal: true,
                type: 'iframe'
            });
        },
        resizeOverlayer: function () {
            if (window.document === window.parent.document) {
                return;
            }

            var magnificPopup = window.parent.jQuery.magnificPopup.instance;
            if (!magnificPopup) {
                return;
            }

            var resize = function () {
                magnificPopup.content[0].style.height = ($(document).height() + 10) + 'px';
            };

            resize();
            $(window).load(resize);
        },
        ELEMENT_SELECTOR: '.js-recatWfOverlay',
        ELEMENT_CLOSE_SELECTOR: '.js-webformClose',
        CLICK_EVENT: 'click.recatWfOverlay'
    };
}) (jQuery);