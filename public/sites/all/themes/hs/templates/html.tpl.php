<?php print $doctype; ?>
<!--[if lte IE 7]> <html class="ie ie7" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>" <?php print $rdf->version . $rdf->namespaces; ?>> <![endif]-->
<!--[if IE 8]> <html class="ie" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>" <?php print $rdf->version . $rdf->namespaces; ?>> <![endif]-->
<!--[if gt IE 8]><!--> <html lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>" <?php print $rdf->version . $rdf->namespaces; ?>> <!--<![endif]-->
    <head<?php print $rdf->profile; ?>>
        <title><?php print $head_title; ?></title>
        <?php print $head; ?>
        <?php print $styles; ?>

        <?php if (!empty($modernizr)) {
            print $modernizr;
        } ?>

        <?php if (!empty($html5shim)) {
            print $html5shim;
        } ?>
    </head>
    <body class="<?php print $classes; ?>" <?php print $attributes; ?>>
        <?php print $page_top; ?>
        <?php print $page; ?>
        <?php print $page_bottom; ?>
        <?php print $scripts; ?>
    </body>
</html>