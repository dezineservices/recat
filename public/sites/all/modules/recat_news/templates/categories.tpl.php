<ul class="menu">
    <?php foreach ($categories as $category): ?>
    <li>
        <a href="<?php print url(sprintf('taxonomy/term/%d', $category['term']->tid)); ?>">
            <?php print $category['term']->name; ?> (<?php print $category['count']; ?>)
        </a>
    </li>
    <?php endforeach; ?>
</ul>