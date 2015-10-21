<ul class="menu">
    <?php foreach ($categories as $category): ?>
    <li>
        <?php $text = sprintf('%s (%d)', $category['term']->name, $category['count']); ?>
        <?php print l($text, sprintf('taxonomy/term/%d', $category['term']->tid)); ?>
    </li>
    <?php endforeach; ?>
</ul>