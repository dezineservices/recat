<div class="block panel-sidebar panel-filter">
    <h6 class="title"><?php print $title; ?></h6>
    <div class="content">
        <?php foreach ($tags as $tag): ?>
        <label class="label-winput">
            <input type="checkbox" name="filter" value="<?php print $tag->tid; ?>" />
            <?php print $tag->name; ?>
        </label>
        <?php endforeach; ?>
    </div>
</div>

