(function () {
    'use strict';

    angular
        .module('recatActivity')
        .service('core.service.library', activityService);

    activityService.$inject = ['$http', '$q'];

    function activityService ($http, $q) {
        return {
            getActivities: getActivities
        };

        function getActivities (page, tid) {
            return loadData(page, tid);
        }

        function loadData (page, tid) {
            var params = {};
            if (page) {
                params.page = parseInt(page, 10);
            }

            if (tid) {
                params.tid = tid;
            }

            var request = $http({
                url: Drupal.settings.recatActivity.serviceUrl,
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
