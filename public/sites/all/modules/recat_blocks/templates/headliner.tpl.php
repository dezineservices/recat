<span class="bg" style="background-image: url('<?php print $image_url; ?>');"></span>

<div class="tbl width-full">
    <div class="tbl-c valign">
        <div class="row">
            <div class="columns small-12 medium-8 medium-push-2">
                <h1 class="title"><?php print $title; ?></h1>
                <p class="content"><?php print $content; ?></p>

                <p class="actions">
                    <a href="<?php print $url; ?>" data-target="<?php print str_replace('#', '', $url); ?>" class="btn-border"><?php print $cta; ?></a>
                </p>
            </div>
        </div>
    </div>
</div>

<span class="icons sprite-before sprite-after">
    <i class="icon sprite"></i>
</span>