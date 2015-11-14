<?php if (isset($prefix)): ?>
    <?php print $prefix; ?>
<?php endif; ?>

<?php foreach ($items as $delta => $item): ?>
    <p><?php print render($item); ?></p>
<?php endforeach; ?>

<?php if (isset($suffix)): ?>
    <?php print $suffix; ?>
<?php endif; ?>