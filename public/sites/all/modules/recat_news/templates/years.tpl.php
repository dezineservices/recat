<ul class="menu">
    <?php foreach ($years as $year): ?>
        <li>
            <?php $text = sprintf('%s (%d)', $year['label'], $year['count']); ?>
            <?php print l($text, $year['url']); ?>
        </li>
    <?php endforeach; ?>
</ul>