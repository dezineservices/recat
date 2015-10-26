<div class="row" data-equalheight>
    <?php foreach ($boxes as $box): ?>
    <div class="columns small-12 medium-4">
        <?php print render($box); ?>
    </div>
    <?php endforeach; ?>
</div>