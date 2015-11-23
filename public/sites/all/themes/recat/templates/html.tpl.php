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

    <script>
        (function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){
            (i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),
            m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)
        })(window,document,'script','//www.google-analytics.com/analytics.js','ga');

        ga('create', 'UA-70505247-1', 'auto');
        ga('send', 'pageview');
    </script>
</head>
<body class="<?php print $classes; ?>" <?php print $attributes; ?>>
<?php print $page_top; ?>
<?php print $page; ?>
<?php print $page_bottom; ?>
<?php print $scripts; ?>
</body>
</html>