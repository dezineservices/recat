<h1 class="h2<?php print $subtitle ? ' with-subtitle' : ''; ?>"><?php print $title; ?></h1>
<?php if ($subtitle): ?>
<h2 class="h3"><?php print $subtitle; ?></h2>
<?php endif; ?>

<?php print $content; ?>