var RecatEqualHeight = (function (el, $) {
    var resizeTimeout = null;

    var resizeByContext = function (el) {
        var i = 0,
            len = el.length;

        for (; i < len; i += 1) {
            resize(el[i].querySelectorAll('[data-equalheight-part]'));
        }
    };

    var resize = function (el) {
        var elementsCache = {},
            elementsCacheMapper = [],
            i = 0,
            len = el.length,
            offsetTop = 0,
            height = 0;

        for (; i < len; i += 1) {
            el[i].style.height = null;

            offsetTop = el[i].offsetTop;
            height = el[i].clientHeight;

            if (!elementsCache[offsetTop]) {
                elementsCache[offsetTop] = {
                    el: [],
                    maxHeight: 0
                };

                elementsCacheMapper.push(offsetTop);
            }

            elementsCache[offsetTop].el.push(el[i]);
            if (elementsCache[offsetTop].maxHeight < height) {
                elementsCache[offsetTop].maxHeight = height;
            }
        }

        i = 0;
        len = elementsCacheMapper.length;

        for (; i < len; i += 1) {
            if (elementsCache[elementsCacheMapper[i]].maxHeight === 0) {
                continue;
            }

            $(elementsCache[elementsCacheMapper[i]].el).height(
                elementsCache[elementsCacheMapper[i]].maxHeight);
        }

    };

    var onLoad = function () {
        resizeByContext(el);
    };

    var onResize = function () {
        resizeByContext(el);
    };

    if (el) {
        resizeByContext(el);
    }

    $(window).load(onLoad);
    $(window).resize(function () {
        if (resizeTimeout) {
            clearTimeout(resizeTimeout);
        }

        resizeTimeout = setTimeout(onResize, 100);
    });

    return {
        resizeByContext: resizeByContext,
        resize: resize
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