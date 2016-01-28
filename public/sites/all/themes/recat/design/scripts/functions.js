jQuery(document).foundation();

var RecatEqualHeight = (function (el, $) {
    'use strict';

    var resizeTimeout = null,
        currentWindowWidth = 0,
        elementsCache = {};

    var resizeByContext = function (el) {
        var i = 0,
            len = el.length;

        for (; i < len; i += 1) {
            resize(getArray(el[i].querySelectorAll('[data-equalheight-part]')).reverse());
        }
    };

    var resize = function (el) {
        var i = 0,
            len = el.length,
            offsetTop = 0,
            height = 0;

        for (; i < len; i += 1) {
            if (el[i].equalHeightReset) {
                el[i].style.height = null;
            }

            height = $(el[i]).height();
            offsetTop = $(el[i]).offset().top;

            if (!elementsCache[offsetTop]) {
                elementsCache[offsetTop] = {
                    el: [],
                    maxHeight: 0
                }
            }

            elementsCache[offsetTop].el.push(el[i]);
            if (elementsCache[offsetTop].maxHeight < height) {
                elementsCache[offsetTop].maxHeight = height;
            }

            if (elementsCache[offsetTop].el.length > 1) {
                el.splice(i, 1);

                equalize(elementsCache[offsetTop]);
                resize(el);

                break;
            }
        }
    };

    var equalize = function (row) {
        var i = 0,
            len = row.el.length;

        if (row.maxHeight === 0) {
            return;
        }

        for (; i < len; i += 1) {
            row.el[i].equalHeightReset = false;
            $(row.el[i]).height(row.maxHeight);
        }
    };

    var getArray = function (el) {
        var array = [],
            i = 0,
            len = el.length;

        for (; i < len; i += 1) {
            el[i].equalHeightReset = true;
            array.push(el[i]);
        }

        return array;
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

    if (el) {
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
        resize: resize,
        forceResize: onResize
    };
}) (document.querySelectorAll('[data-equalheight]'), jQuery);

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