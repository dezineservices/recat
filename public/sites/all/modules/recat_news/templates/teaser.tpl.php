<article class="panel panel-image">
    <?php print $image; ?>

    <div class="inner">
        <span class="label"><?php print $post_date; ?></span>
        <h1 class="title">
            <a href="<?php print url($url['path']); ?>"><?php print $title; ?></a>
        </h1>
        <p class="content"><?php print $content; ?></p>

        <a href="<?php print url($url['path']); ?>" class="btn"><?php print $cta; ?></a>
    </div>
</article>