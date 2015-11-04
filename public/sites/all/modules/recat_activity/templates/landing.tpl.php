<div class="row">
    <div id="sidebar" class="columns-sidebar columns small-12 medium-3">
        <?php print drupal_render($blocks); ?>
    </div>
    <div id="systemMain" class="columns-content columns small-12 medium-9 large-push-1 large-7 end">
        <?php foreach ($terms as $term): ?>
            <?php print $term; ?>
        <?php endforeach; ?>
    </div>
</div>
