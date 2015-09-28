
(function ($) {

Drupal.behaviors.hsAdmin = {
    attach: function (context, settings) {
        var previewUrl = settings.hsAdmin.contentPreview.path,
            bringToTop = settings.hsAdmin.contentPreview.bringToTop;

        this.openPopup(previewUrl, bringToTop);
    },

    openPopup: function (url, bringToTop) {
        var title = 'Node edit preview',
            features = 'dialog=1,scrollbars=1,menubar=0,toolbar=0,location=0,status=0',
            popup = window.open('', title, features);

        // Bring popup to top, by putting a focus on it
        if (bringToTop) {
            var existingWindow = popup.location != 'about:blank',
                isChrome = /chrom(e|ium)/.test(navigator.userAgent.toLowerCase());

            if (existingWindow) {
                if (isChrome) {
                    // Chrome workaround: force focus on popup
                    popup.close();
                    popup = this.openPopup(url, bringToTop);
                } else {
                    popup.focus();
                }
            }
        }

        // (Re)load the popup contents
        popup.location = url;

        return popup;
    }
};

})(jQuery);
