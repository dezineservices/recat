<header id="header" role="banner">
    <div class="row">
        <div class="columns small-12 medium-4">
            <a href="<?php print $front_page; ?>" title="<?php print $site_name; ?>" id="logo">
                <img src="<?php print $logo; ?>" alt="<?php print $site_slogan; ?>" />
            </a>
        </div>
        <div class="columns small-12 medium-8">
            <?php if ($main_menu): ?>
            <nav id="mainNav" role="navigation">
                <?php print render($main_menu); ?>
            </nav>
            <?php endif; ?>
        </div>
        <div class="columns small-12">
            <?php print render($page['header']); ?>
        </div>
    </div>
</header>

<div id="main" role="main">
    <div class="row">
        <div class="columns small-12">
            <?php if ($breadcrumb): ?>
                <div id="breadcrumb"><?php print $breadcrumb; ?></div>
            <?php endif; ?>
            <?php print $messages; ?>
            <?php print render($title_prefix); ?>
            <?php if ($title): ?>
                <h1 id="pageTitle"><?php print $title; ?></h1>
            <?php endif; ?>
            <?php print render($title_suffix); ?>
            <?php if (!empty($tabs)): ?>
                <div class="tabs"><?php print render($tabs); ?></div>
            <?php endif; ?>
            <?php print render($page['help']); ?>
            <?php if ($action_links): ?>
                <ul class="action-links"><?php print render($action_links); ?></ul>
            <?php endif; ?>
            <?php print render($page['content']); ?>
        </div>
    </div>
</div>

<footer id="footer" role="contentinfo">
    <div class="row">
        <div class="columns small-12">
            <?php print render($page['footer']); ?>
        </div>
    </div>
</footer>