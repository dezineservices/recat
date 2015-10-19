<div class="tags clearfix">
    <?php foreach ($tags as $tag): ?>
    <a class="tag" href="<?php print url(sprintf('taxonomy/term/%d', $tag->tid)); ?>">
        <?php print $tag->name; ?>
    </a>
    <?php endforeach; ?>
</div>