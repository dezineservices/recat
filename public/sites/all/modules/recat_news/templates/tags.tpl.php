<div class="tags clearfix">
    <?php foreach ($tags as $tag): ?>
    <?php print l($tag->name, sprintf('taxonomy/term/%d', $tag->tid), array(
        'attributes' => array('class' => array('tag')),
    )); ?>
    <?php endforeach; ?>
</div>