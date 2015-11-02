<div class="activity relative ag-app" data-ng-app="recatActivity" data-ng-controller="ListingController as listing" data-ng-class="{ loaded: listing.loaded }">
    <table class="tbl-listing tbl-activity" cellspacing="0" cellpadding="0" data-ng-class="{ 'tbl-hover': listing.nodes.length }">
        <thead>
        <tr>
            <th class="fixed-size" data-ng-non-bindable><?php print _hs_resource_get('recat_activity.table.status'); ?></th>
            <th data-ng-non-bindable><?php print _hs_resource_get('recat_activity.table.continent'); ?></th>
            <th data-ng-non-bindable><?php print _hs_resource_get('recat_activity.table.country'); ?></th>
            <th data-ng-non-bindable><?php print _hs_resource_get('recat_activity.table.name.' . $term->tid); ?></th>
        </tr>
        </thead>
        <tbody data-ng-if="listing.nodes.length">
        <tr data-ng-repeat="node in listing.nodes track by node.nid" data-ng-click="listing.goToPage(node)">
            <td><span class="state {{ node.statusClass }}">{{ node.status }}</span></td>
            <td>{{ node.continent }}</td>
            <td>{{ node.country }}</td>
            <td class="font-regular"><a data-ng-href="{{ node.url }}">{{ node.title }}</a></td>
        </tr>
        </tbody>
        <tbody data-ng-if="!listing.nodes.length">
        <tr>
            <td colspan="4" class="error">{{ listing.errorMessage }}</td>
        </tr>
        </tbody>
    </table>

    <div class="item-list partial" data-ng-if="listing.totalPages > 1">
        <ul class="pager">
            <li class="pager-previous first" data-ng-show="listing.currentPage > 0">
                <a data-ng-click="listing.pagePrevious()"><?php print t('prev'); ?></a>
            </li>
            <li class="pager-item" data-ng-repeat="page in listing.pages track by $index" data-ng-switch="page">
                <span data-ng-switch-when="ellipsis">...</span>
                <a data-ng-switch-default data-ng-click="listing.pageChange(page)" data-ng-class="{ 'pager-current': listing.currentPage === page - 1 }">{{ page }}</a>
            </li>
            <li class="pager-next last" data-ng-show="listing.currentPage < listing.totalPages - 1">
                <a data-ng-click="listing.pageNext()"><?php print t('next'); ?></a>
            </li>
        </ul>
    </div>

    <span class="loader" data-ng-show="listing.loading"></span>
</div>