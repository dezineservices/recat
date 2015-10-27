<?php

function recat_news_taxonomy_page($term) {
    $settings = _recat_news_get_settings();
    $teasers = array();

    if ($nids = taxonomy_select_nodes($term->tid, TRUE, variable_get('default_nodes_main', 10))) {
        foreach (node_load_multiple($nids) as $nid => $node) {
            $teasers[] =
                _recat_news_get_teaser_view($node, $settings['image_style'], $settings['date_format'], $settings['max_chars']);
        }
    }

    return array(
        'teasers' => array(
            '#theme' => 'recat_news_listing',
            '#title' => str_replace('!taxonomy_name', $term->name, _hs_resource_get('recat_news.listing.results_title')),
            '#teasers' => $teasers,
            '#item_per_line' => 2,
        ),
        'pager' => _recat_core_get_pager_build(),
    );
}