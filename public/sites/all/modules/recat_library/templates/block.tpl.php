<div class="row" data-recat-app="recatLibrary" data-ng-controller="ListingController as listing" data-instance="<?php print $instance_id; ?>">
    <?php if ($tags): ?>
    <div class="columns-sidebar columns small-12 medium-3">
        <?php print drupal_render($tags); ?>
    </div>
    <div class="columns-content columns small-12 medium-9 large-push-1 large-8 end">
    <?php else: ?>
    <div class="columns small-12">
    <?php endif; ?>
        <?php if ($has_search): ?>
        <div class="row">
            <div class="columns small-12 medium-4">
                <?php $placeholder = _hs_resource_get('recat_library.search.placeholder', 'plain', null, false); ?>
                <input type="text" name="librarySearch" data-ng-model="listing.searchQuery" data-ng-change="listing.queryChange();" placeholder="{{ listing.searchQuery ? listing.searchQuery : '<?php print $placeholder; ?>' }}" />
            </div>
        </div>
        <?php endif; ?>
        <div class="library relative ag-app" data-ng-class="{ loaded: listing.loaded }">
            <table class="tbl-listing tbl-library" cellspacing="0" cellpadding="0" data-ng-class="{ 'tbl-hover': listing.nodes.length }">
                <thead>
                    <tr>
                        <th class="icon-column"><i class="sprite icon-lock"></i></th>
                        <th>
                            <a class="sorting none-selectable" data-ng-click="listing.sortingChange('title');" data-ng-class="{ active: listing.isSortingActive('title'), up: listing.isSortingOrderDesc('title') }">
                                <?php print _hs_resource_get('recat_library.table.document_title', 'plain', null, false); ?>
                            </a>
                        </th>
                        <th>
                            <a class="sorting none-selectable" data-ng-click="listing.sortingChange('tags');" data-ng-class="{ active: listing.isSortingActive('tags'), up: listing.isSortingOrderDesc('tags') }">
                                <?php print _hs_resource_get('recat_library.table.tags', 'plain', null, false); ?>
                            </a>
                        </th>
                        <th>
                            <a class="sorting none-selectable" data-ng-click="listing.sortingChange('date');" data-ng-class="{ active: listing.isSortingActive('date'), up: listing.isSortingOrderDesc('date') }">
                                <?php print _hs_resource_get('recat_library.table.date', 'plain', null, false); ?>
                            </a>
                        </th>
                        <th><?php print _hs_resource_get('recat_library.table.file_extension', 'plain', null, false); ?></th>
                    </tr>
                </thead>
                <tbody data-ng-if="listing.nodes.length">
                    <tr data-ng-repeat="node in listing.nodes track by node.nid" data-ng-click="listing.downloadFile(node)">
                        <td class="icon-column"><i class="sprite icon-lock" data-ng-class="{ unlocked: !node.private, locked: node.private }"></i></td>
                        <td>
                            <a data-ng-if="!listing.isFirstDownloadRequest" data-ng-href="{{ node.url }}" data-ng-class="{ 'js-recatWfOverlay': node.private }">{{ node.title }}</a>
                            <a class="js-recatWfOverlay" data-ng-if="listing.isFirstDownloadRequest" data-ng-href="{{ listing.firstDownloadRequestUrl }}" data-ng-click="listing.requestFile()">{{ node.title }}</a>
                        </td>
                        <td>{{ node.tags }}</td>
                        <td>{{ node.date }}</td>
                        <td>{{ node.file_extension }}</td>
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
    </div>
</div>