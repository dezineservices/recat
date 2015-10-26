<li class="<?php print $classes; ?>"<?php print $attributes; ?>>
    <?php print render($title_prefix); ?>
    <h3 class="search-title h6"<?php print $title_attributes; ?>>
        <a href="<?php print $url; ?>"><?php print $title; ?></a>
    </h3>
    <?php print render($title_suffix); ?>
    <p class="search-snippet"<?php print $content_attributes; ?>><?php print $snippet; ?></p>
</li>
