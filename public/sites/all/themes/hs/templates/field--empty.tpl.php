<?php if (isset($prefix)): ?>
    <?php print $prefix; ?>
<?php endif; ?>

<?php foreach ($items as $delta => $item): ?>
    <?php print render($item); ?>
<?php endforeach; ?>

<?php if (isset($suffix)): ?>
    <?php print $suffix; ?>
<?php endif; ?>