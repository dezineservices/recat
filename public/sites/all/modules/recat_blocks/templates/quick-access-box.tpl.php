<article class="panel text-center">
    <span class="icon-bg">
        <i class="icon <?php print $icon; ?> sprite"></i>
    </span>

    <h1 class="title"><?php print $title; ?></h1>
    <p class="content narrow"><?php print $content; ?></p>

    <?php if ($url): ?>
    <p>
        <a href="<?php print url($url); ?>" class="more-info uppercase"><?php print $more_info; ?></a>
    </p>
    <?php endif; ?>
</article>