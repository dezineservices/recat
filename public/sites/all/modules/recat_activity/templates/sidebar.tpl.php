<ul class="menu">
    <?php foreach ($terms as $term): ?>
    <li<?php print count($term->css_list_classes) ? ' class="' . implode(' ', $term->css_list_classes) . '"' : '' ;?>>
        <a href="<?php print url(sprintf('taxonomy/term/%d', $term->tid)); ?>"<?php print count($term->css_classes) ? ' class="' . implode(' ', $term->css_classes) . '"' : '' ;?>>
            <?php print $term->name; ?>
        </a>

        <?php if (isset($term->terms)): ?>
        <ul>
            <?php foreach ($term->terms as $term): ?>
            <li>
                <a href="<?php print url(sprintf('taxonomy/term/%d', $term->tid)); ?>"<?php print count($term->css_classes) ? ' class="' . implode(' ', $term->css_classes) . '"' : '' ;?>>
                    <?php print $term->name; ?>
                    <span class="counter"><?php print $term->activities_count; ?></span>
                </a>
            </li>
            <?php endforeach; ?>
        </ul>
        <?php endif; ?>
    </li>
    <?php endforeach; ?>
</ul>