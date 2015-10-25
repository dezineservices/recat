<div id="mainOverlay" role="main">
    <span class="close sprite js-webformClose"></span>

    <?php print render($title_prefix); ?>
    <h1 class="title h5"><?php print $title; ?></h1>
    <?php print render($title_suffix); ?>
    
    <?php if (!empty($messages)): ?>
    <div id="messages">
        <div class="page"><?php print $messages; ?></div>
    </div>
    <?php endif; ?>
    
    <?php print render($page['content']); ?>
</div>