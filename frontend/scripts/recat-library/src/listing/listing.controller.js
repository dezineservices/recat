(function () {
    'use strict';

    angular
        .module('recatLibrary')
        .controller('ListingController', ListingController);

    ListingController.$inject = ['$scope', '$element', '$attrs', 'core.service.library'];

    function ListingController ($scope, $element, $attributes, service) {
        var MAX_PAGER_BUTTONS = 9;

        var vm = this,
            currentSettings = getCurrentSettings($attributes.instance),
            currentTags = [],
            currentCondition = currentSettings.condition,
            currentSortColumn = '',
            currentSortOrder = '',
            isFirstLoad = true;

        vm.loaded = false;
        vm.loading = true;

        vm.currentPage = 0;
        vm.totalPages = 0;
        vm.pages = [];
        vm.nodes = [];
        vm.searchQuery = '';
        vm.errorMessage = '';

        vm.pagePrevious = pagePrevious;
        vm.pageNext = pageNext;
        vm.pageChange = pageChange;
        vm.filterChange = filterChange;
        vm.sortingChange = sortingChange;
        vm.queryChange = queryChange;
        vm.downloadFile = downloadFile;
        vm.requestFile = requestFile;

        vm.isSortingActive = isSortingActive;
        vm.isSortingOrderDesc = isSortingOrderDesc;

        vm.isFirstDownloadRequest = isFirstDownloadRequest();
        vm.firstDownloadRequestUrl = currentSettings.overlayUrl;

        if (currentSettings.tid) {
            var tids = currentSettings.tid.split(',');
            for (var i = 0, len = tids.length; i < len; i += 1) {
                currentTags.push(tids[i]);
            }
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

        function filterChange ($event, id) {
            var isChecked = !!$event.target.checked;

            if (isChecked) {
                currentTags.push(id);
            } else {
                currentTags = removeTag(id);
            }

            loadData();
        }

        function sortingChange (sortColumn) {
            if (currentSortColumn === sortColumn) {
                switch (currentSortOrder) {
                    case 'asc':
                        currentSortOrder = 'desc';
                        break;
                    case 'desc':
                        currentSortOrder = 'asc';
                }
            } else {
                currentSortColumn = sortColumn;
                currentSortOrder = 'asc';
            }

            loadData();
        }

        function queryChange () {
            loadData();
        }

        function downloadFile (file) {
            if (isFirstDownloadRequest()) {
                Drupal.behaviors.recatWfOverlay.openOverlayer(
                    currentSettings.overlayUrl);

                handleOverlay();
                return;
            }

            if (!file.url) {
                return;
            }

            if (!file.private || !doesOverlayExists()) {
                document.location = file.url;
                return;
            }

            Drupal.behaviors.recatWfOverlay.openOverlayer(file.url);
        }

        function requestFile () {
            handleOverlay(true);
            setTimeout(handleOverlay, 100);
        }

        function loadData () {
            handleOverlay(true);

            vm.loading = true;

            service.getFiles(vm.searchQuery, vm.currentPage, currentCondition, currentSortColumn, currentSortOrder, currentTags)
                .then(handleSuccess, handleError);
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

        function removeTag (idToRemove) {
            var tags = [],
                i = 0,
                len = currentTags.length;

            for (; i < len; i += 1) {
                if (idToRemove === currentTags[i]) {
                    continue;
                }

                tags.push(currentTags[i]);
            }

            return tags;
        }

        function handleSuccess (data) {
            vm.loaded = true;
            vm.loading = false;

            vm.nodes = data.nodes;
            vm.currentPage = data.pager.current;
            vm.totalPages = data.pager.pages;

            vm.pages = generatePager();

            scrollToContext();
            setTimeout(handleOverlay, 100);
        }

        function handleError (errorData) {
            vm.loaded = true;
            vm.loading = false;

            vm.nodes = vm.pages = [];
            vm.currentPage = vm.totalPages = 0;

            vm.errorMessage = errorData.text;
        }

        function handleOverlay (doUnbind) {
            if (!doesOverlayExists()) {
                return;
            }

            if (doUnbind) {
                Drupal.behaviors.recatWfOverlay.detach($element);
                return;
            }

            Drupal.behaviors.recatWfOverlay.attach($element);
        }

        function doesOverlayExists () {
            return Drupal && Drupal.behaviors && Drupal.behaviors.recatWfOverlay;
        }

        function isFirstDownloadRequest () {
            if (!doesOverlayExists()) {
                return false;
            }

            return !getCookie(currentSettings.cookieName);
        }

        function isSortingActive (sortingColumn) {
            return currentSortColumn === sortingColumn;
        }

        function isSortingOrderDesc (sortingColumn) {
            return currentSortOrder === 'desc' && isSortingActive(sortingColumn);
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

        function getCookie (name) {
            var value = '; ' + document.cookie;
            var parts = value.split('; ' + name + '=');
            if (parts.length === 2) return parts.pop().split(';').shift();
        }

        function getCurrentSettings (instanceName) {
            if (!instanceName || !Drupal.settings[instanceName]) {
                return Drupal.settings.recatLibrary;
            }

            return angular.extend(Drupal.settings[instanceName], Drupal.settings.recatLibrary);
        }
    }
})();
