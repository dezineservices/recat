<?php if (count($teasers)): ?>
    <?php if ($title): ?>
        <h1 class="h3"><?php print $title; ?></h1>
    <?php endif; ?>

    <?php $counter = 0; $is_closed = false; $is_last = false; $medium_column = 12 / $item_per_line; ?>

    <?php foreach ($teasers as $teaser): $is_last = $counter === count($teasers) - 1; ?>
    <?php if ($counter % $item_per_line === 0): $is_closed = false; ?>
    <div class="row">
    <?php endif; ?>

        <div class="columns small-12 medium-<?php print $medium_column; ?><?php print $is_last ? ' end': ''; ?>">
            <?php print render($teaser); ?>
        </div>

    <?php if (($counter + 1) % $item_per_line === 0): $is_closed = true; ?>
    </div>
    <?php endif; ?>
    <?php ++$counter; endforeach; ?>

    <?php if(!$is_closed): ?>
    </div>
    <?php endif; ?>
<?php else: ?>
    <article class="node node-news panel panel-node clearfix">
        <?php if ($title): ?>
            <h1 class="h3"><?php print $title; ?></h1>
        <?php endif; ?>

        <p><?php print _hs_resource_get('recat_news.listing.no_results'); ?></p>
    </article>
<?php endif; ?>