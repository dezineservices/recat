(function () {
    'use strict';

    angular
        .module('recatActivity')
        .controller('ListingController', ListingController);

    ListingController.$inject = ['$element', 'core.service.library'];

    function ListingController ($element, service) {
        var MAX_PAGER_BUTTONS = 9;

        var vm = this,
            currentTid = null,
            isFirstLoad = true;

        vm.loaded = false;
        vm.loading = true;

        vm.currentPage = 0;
        vm.totalPages = 0;
        vm.pages = [];
        vm.nodes = [];
        vm.errorMessage = '';

        vm.pagePrevious = pagePrevious;
        vm.pageNext = pageNext;
        vm.pageChange = pageChange;
        vm.goToPage = goToPage;

        if (Drupal.settings.recatActivity.tid) {
            currentTid = Drupal.settings.recatActivity.tid;
        }

        loadData();

        function pagePrevious () {
            vm.currentPage -= 1;
            loadData();
        }

        function pageNext () {
            vm.currentPage += 1;
            loadData();
        }

        function pageChange (page) {
            vm.currentPage = parseInt(page, 10) - 1;
            loadData();
        }

        function goToPage (node) {
            document.location = node.url;
            return;
        }

        function loadData () {
            vm.loading = true;
            service.getActivities(vm.currentPage, currentTid).then(handleSuccess, handleError);
        }

        function generatePager () {
            var pages = [],
                middle = Math.ceil(MAX_PAGER_BUTTONS / 2),
                current = vm.currentPage + 1,
                first = current - middle + 1,
                last = current + MAX_PAGER_BUTTONS - middle,
                max = vm.totalPages;

            var i = first;
            if (last > max) {
                i = i + (max - last);
                last = max;
            }

            if (i <= 0) {
                last = last + (1 - i);
                i = 1;
            }

            if (i > 1) {
                pages.push('ellipsis');
            }

            for (; i <= last && i <= max; i += 1) {
                pages.push(i);
            }

            if (i < max) {
                pages.push('ellipsis');
            }

            return pages;
        }

        function handleSuccess (data) {
            vm.loaded = true;
            vm.loading = false;

            vm.nodes = data.nodes;
            vm.currentPage = data.pager.current;
            vm.totalPages = data.pager.pages;

            vm.pages = generatePager();

            scrollToContext();
        }

        function handleError (errorData) {
            vm.loaded = true;
            vm.loading = false;

            vm.nodes = vm.pages = [];
            vm.currentPage = vm.totalPages = 0;

            vm.errorMessage = errorData.text;

            scrollToContext();
        }

        function scrollToContext () {
            if (isFirstLoad) {
                isFirstLoad = false;
                return;
            }

            jQuery('html, body').animate({
                scrollTop: $element.offset().top
            }, 400);
        }
    }
})();
