(function () {
    'use strict';

    angular
        .module('recatLibrary')
        .controller('ListingController', ListingController);

    ListingController.$inject = ['$scope', 'core.service.library'];

    function ListingController ($scope, service) {
        var MAX_PAGER_BUTTONS = 2;

        var vm = this;

        vm.currentPage = 0;
        vm.totalPages = 0;
        vm.pages = [];
        vm.nodes = [];

        vm.pagePrevious = pagePrevious;
        vm.pageNext = pageNext;
        vm.pageChange = pageChange;

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
            vm.currentPage = parseInt(page, 10);
            loadData();
        }

        function loadData () {
            service.getFiles(vm.currentPage).then(handleSuccess, handleError);
        }

        function generatePager () {
            var pages = [],
                middle = Math.ceil(MAX_PAGER_BUTTONS / 2),
                first = vm.currentPage - middle + 1,
                last = vm.currentPage + MAX_PAGER_BUTTONS - middle,
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

            for (; i <= last && i <= max; i += 1) {
                pages.push(i);
            }

            if (i < max) {
                pages.push('ellipsis');
            }

            return pages;
        }

        function handleSuccess (data) {
            vm.nodes = data.nodes;
            vm.currentPage = data.pager.current;
            vm.totalPages = data.pager.pages;

            vm.pages = generatePager();
        }

        function handleError (errorMessage) {
            //@todo - provide error;
            console.log(errorMessage);
        }
    }
})();
