<header id="header" role="banner">
    <div class="row">
        <div class="columns small-12 medium-2">
            <a href="<?php print $front_page; ?>" title="<?php print $site_name; ?>" class="site-logo" id="logo">
                <img src="<?php print $logo; ?>" alt="<?php print $site_slogan; ?>" />
                <!--<//?php if (isset($site_name_abbr)): ?>
                    <//?php print $site_name_abbr; ?>
                <//?php endif; ?>-->
            </a>
        </div>
        <div class="columns small-12 medium-7">
            <?php if ($main_menu): ?>
            <nav id="mainNav" class="clearfix" role="navigation">
                <?php print render($main_menu); ?>
            </nav>
            <?php endif; ?>
        </div>
        <div class="columns small-3">
            <?php print render($page['header']); ?>
        </div>
    </div>
</header>

<?php print render($page['content_top']); ?>

<?php if (!$is_front && $title): ?>
<section id="pageTitle">
    <div class="row">
        <div class="columns small-12">
            <?php if (isset($heading_title)): ?>
                <div class="h1 title"><?php print $heading_title; ?></div>
            <?php else: ?>
                <?php print render($title_prefix); ?>
                <h1 class="title"><?php print $title; ?></h1>
                <?php print render($title_suffix); ?>
            <?php endif; ?>
        </div>
    </div>
</section>
<?php endif; ?>

<?php print $messages; ?>

<?php if (!empty($tabs)): ?>
<div class="authoring-tabs">
    <div class="row">
        <div class="columns small-12">
            <?php print render($tabs); ?></div>
        </div>
    </div>
</div>
<?php endif; ?>

<?php if ($has_main_content || isset($block_reference)): ?>
<section id="main" role="main"<?php print isset($is_colored) ? ' class="colored"' : ''; ?>>
    <?php if ($has_main_content): ?>
    <div class="row">
        <div id="sidebar" class="columns-sidebar columns small-12 medium-3">
            <?php print render($page['sidebar']); ?>
        </div>
        <div id="content" class="columns-content columns small-12 medium-9 large-push-1 large-7 end">
            <?php print render($page['content']); ?>
        </div>
    </div>
    <?php endif; ?>
    <?php if (isset($block_reference)): ?>
        <?php print drupal_render($block_reference); ?>
    <?php endif; ?>
</section>
<?php endif; ?>

<?php print render($page['content_bottom']); ?>

<footer id="footer" role="contentinfo">
    <div class="row">
        <div class="columns small-12 medium-2">
            <a href="<?php print $front_page; ?>" title="<?php print $site_name; ?>" class="site-logo">
                <img src="<?php print $logo; ?>" alt="<?php print $site_slogan; ?>" />
                <?php if (isset($site_name_abbr)): ?>
                    <?php print $site_name_abbr; ?>
                <?php endif; ?>
            </a>
        </div>
        <div class="columns small-12 medium-8 end">
            <?php if ($footer_menu): ?>
                <nav id="footerNav" class="clearfix">
                    <?php print render($footer_menu); ?>
                </nav>
            <?php endif; ?>
        </div>
    </div>
    <?php if (isset($footer_copyright)): ?>
    <div class="row">
        <div class="columns small-12">
            <div class="copyright">
                <?php print $footer_copyright; ?>
            </div>
        </div>
    </div>

    <?php endif; ?>
</footer>