<div class="row">
    <div class="columns-sidebar columns small-12 medium-3">
        <?php print drupal_render($tags); ?>
    </div>
    <div class="columns-content columns small-12 medium-9 large-push-1 large-8 end">
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
                <tr>
                    <td class="icon-column"><i class="sprite icon-lock unlocked"></i></td>
                    <td>Document title</td>
                    <td>tags</td>
                    <td>01/01/2019</td>
                </tr>
                <tr>
                    <td class="icon-column"><i class="sprite icon-lock locked"></i></td>
                    <td>Document title</td>
                    <td>tags</td>
                    <td>01/01/2019</td>
                </tr>
            </tbody>
        </table>
    </div>
</div>