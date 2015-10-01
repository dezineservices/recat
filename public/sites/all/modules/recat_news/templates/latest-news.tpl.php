<div class="row">
    <?php foreach ($teasers as $teaser): ?>
    <div class="columns small-12 medium-4">
        <?php print render($teaser); ?>
    </div>
    <?php endforeach; ?>
</div>