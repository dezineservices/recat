<?php $counter = 0; $is_closed = false; $is_last = false; ?>

<?php foreach ($teasers as $teaser): $is_last = $counter === count($teasers) - 1; ?>
<?php if ($counter % 3 === 0): $is_closed = false; ?>
<div class="row">
<?php endif; ?>

    <div class="columns small-12 medium-4<?php print $is_last ? ' end': ''; ?>">
        <?php print render($teaser); ?>
    </div>

<?php if (($counter + 1) % 3 === 0): $is_closed = true; ?>
</div>
<?php endif; ?>
<?php ++$counter; endforeach; ?>

<?php if(!$is_closed): ?>
</div>
<?php endif; ?>