<article id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
    <?php print render($content['links']); ?>

    <?php print render($title_prefix); ?>
    <?php if ($page && isset($node->has_heading_title)): ?>
    <h1 class="h2"><?php print $title; ?></h1>
    <?php endif; ?>
    <?php if (!$page): ?>
        <h1<?php print $title_attributes; ?> class="node-title">
            <?php print $title; ?>
            <?php if (isset($content['field_suffix'])): ?>
                <em class="title-suffix"><?php print render($content['field_suffix']); ?></em>
            <?php endif; ?>
        </h1>
    <?php endif; ?>
    <?php print render($title_suffix); ?>

    <?php
    hide($content['comments']);
    print render($content);
    ?>
</article>