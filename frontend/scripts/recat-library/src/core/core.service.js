(function () {
    'use strict';

    angular
        .module('recatLibrary')
        .service('core.service.library', libraryService);

    libraryService.$inject = ['$http', '$q'];

    function libraryService ($http, $q) {
        return {
            getFiles: getFiles
        };

        function getFiles (searchQuery, page, condition, sortColumn, sortOrder, tags) {
            return loadData(searchQuery, page, condition, sortColumn, sortOrder, tags);
        }

        function loadData (searchQuery, page, condition, sortColumn, sortOrder, tags) {
            var params = {
                searchQuery: searchQuery
            };

            if (page) {
                params.page = parseInt(page, 10);
            }

            if (condition) {
                params.condition = condition;
            }

            if (sortColumn) {
                params.sort = sortColumn;
            }

            if (sortOrder) {
                params.sortOrder = sortOrder;
            }

            if (tags.length) {
                params.tid = tags.join(',');
            }

            var request = $http({
                url: Drupal.settings.recatLibrary.serviceUrl,
                method: 'GET',
                cache: true,
                params: params
            });

            return request.then(handleSuccess, handleError);
        }

        function handleSuccess (response) {
            if (response.status !== 200) {
                return $q.reject(response.statusText);
            }

            if (!response.data.success) {
                return $q.reject(response.data.message);
            }

            return response.data;
        }

        function handleError (response) {
            if (!angular.isObject(response.data)) {
                return $q.reject('An error occurred');
            }

            return $q.reject(response.statusText);
        }
    }
})();
