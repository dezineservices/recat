<div class="row" data-ng-app="recatLibrary">
    <div class="columns-sidebar columns small-12 medium-3">
        <?php print drupal_render($tags); ?>
    </div>
    <div class="columns-content columns small-12 medium-9 large-push-1 large-8 end" data-ng-controller="ListingController as listing">
        <table class="tbl-library" cellspacing="0" cellpadding="0">
            <thead>
                <tr>
                    <th class="icon-column"><i class="sprite icon-lock"></i></th>
                    <th>Document Title</th>
                    <th>Tags</th>
                    <th>Date</th>
                </tr>
            </thead>
            <tbody>
                <tr data-ng-repeat="node in listing.nodes track by node.nid">
                    <td class="icon-column"><i class="sprite icon-lock" data-ng-class="{ unlocked: !node.private, locked: node.private }"></i></td>
                    <td><a data-ng-href="{{ node.url }}">{{ node.title }}</a></td>
                    <td>{{ node.tags }}</td>
                    <td>{{ node.date }}</td>
                </tr>
            </tbody>
        </table>

        <div class="item-list">
            <ul class="pager">
                <li class="pager-previous first" data-ng-show="listing.currentPage > 0">
                    <a data-ng-click="listing.pagePrevious()"><?php print t('prev'); ?></a>
                </li>
                <li class="pager-item" data-ng-repeat="page in listing.pages track by $index" data-ng-switch="page">
                    <span data-ng-switch-when="ellipsis">...</span>
                    <a data-ng-switch-default data-ng-click="listing.pageChange($index)" data-ng-class="{ 'pager-current': listing.currentPage === page - 1 }">{{ page }}</a>
                </li>
                <li class="pager-next last" data-ng-show="listing.currentPage < listing.totalPages - 1">
                    <a data-ng-click="listing.pageNext()"><?php print t('next'); ?></a>
                </li>
            </ul>
        </div>
    </div>
</div>