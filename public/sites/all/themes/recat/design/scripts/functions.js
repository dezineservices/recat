jQuery(document).foundation();

var RecatEqualHeight = (function (el, $, PART_SELECTOR) {
    'use strict';

    var elementsCache = {},
        resizeTimeout = null,
        currentWindowWidth = 0;

    var processParts = function (el) {
        var i = 0,
            len = el.length,
            offsetTop = 0;

        for (; i < len; i += 1) {
            offsetTop = $(el[i]).offset().top;

            if (!elementsCache[offsetTop]) {
                elementsCache[offsetTop] = {
                    el: [],
                    count: 0,
                    resized: 0
                };
            }

            if (!el[i]._equalized
                || el[i]._offsetTop !== offsetTop) {
                elementsCache[offsetTop].el.push(el[i]);
                elementsCache[offsetTop].count += 1;
            }

            el[i]._equalized = true;
            el[i]._offsetTop = offsetTop;

            if (elementsCache[offsetTop].count > 1
                && elementsCache[offsetTop].resized !== elementsCache[offsetTop].count) {
                elementsCache[offsetTop].resized = elementsCache[offsetTop].el.length;

                processRow(elementsCache[offsetTop].el);
                processParts(el);

                break;
            }
        }
    };

    var processRow = function (parts) {
        if (parts.length < 2) {
            return;
        }

        equalHeight(parts);
    };

    var equalHeight = function (el) {
        var i = 0,
            len = el.length,
            maxHeight = 0;

        for (; i < len; i += 1) {
            maxHeight = Math.max(maxHeight, getHeight($(el[i])));
        }

        if (maxHeight === 0) {
            return;
        }

        $(el).height(maxHeight);
    };

    var getHeight = function (el) {
        return el.height();
    };

    var resetHeight = function (el) {
        el.style.height = null;
    };

    var equalizer = function (parts) {
        if (!parts.length) {
            return;
        }

        var i = 0,
            len = parts.length;

        for (; i < len; i += 1) {
            parts[i]._equalized = false;
            resetHeight(parts[i]);
        }

        processParts(parts);
        elementsCache = {};
    };

    var resizeByContext = function (context) {
        var i = 0,
            len = context.length;

        for (; i < len; i += 1) {
            equalizer(context[i].querySelectorAll(PART_SELECTOR));
        }
    };

    var getWindowWidth = function () {
        var w = window,
            d = document,
            e = d.documentElement,
            g = d.getElementsByTagName('body')[0];

        return w.innerWidth || e.clientWidth || g.clientWidth;
    };

    var onLoad = function () {
        resizeByContext(el);
    };

    var onResize = function () {
        var windowWidth = getWindowWidth();
        if (currentWindowWidth === windowWidth) {
            return;
        }

        elementsCache = {};
        currentWindowWidth = windowWidth;

        resizeByContext(el);
    };

    if (el.length) {
        resizeByContext(el);
    }

    currentWindowWidth = getWindowWidth();

    $(window).load(onLoad);
    $(window).resize(function () {
        if (resizeTimeout) {
            clearTimeout(resizeTimeout);
        }

        resizeTimeout = setTimeout(onResize, 100);
    });

    return {
        resizeByContext: resizeByContext,
        forceResize: onResize
    };
}) (document.querySelectorAll('[data-equalheight]'), jQuery, '[data-equalheight-part]');

var RecatForms = (function (el, $) {
    var uniform = function (el) {
        $(el).uniform();
    };

    if (el) {
        var i = 0,
            len = el.length;

        for (; i < len; i += 1) {
            if (el[i].className.indexOf('js-ratingstars') !== -1) {
                continue;
            }

            uniform(el[i]);
        }
    }

    return {
        uniform: uniform
    };
}) (document.querySelectorAll('input[type="radio"], input[type="checkbox"]'), jQuery);