<?php if (isset($prefix)): ?>
    <?php print $prefix; ?>
<?php endif; ?>

<?php foreach ($items as $delta => $item): ?>
    <div class="<?php print $classes; ?><?php print $delta % 2 ? ' odd' : ' even'; ?><?php print isset($item_classes) && $item_classes[$delta] ? ' ' . implode(' ', $item_classes[$delta]) : '' ?>"<?php print $item_attributes[$delta]; ?>><?php print render($item); ?></div>
<?php endforeach; ?>

<?php if (isset($suffix)): ?>
    <?php print $suffix; ?>
<?php endif; ?>