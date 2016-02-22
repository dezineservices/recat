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

function recat_news_year_page($year) {
    $date = DateTime::createFromFormat('Y', $year);
    if (!$date || strlen($year) !== 4) {
        drupal_not_found();
        drupal_exit();
    }

    $date->setDate($date->format('Y'), 1, 1);
    $date->setTime(0, 0, 0);

    $timestamp_from = $date->getTimestamp();
    $date->add(new DateInterval('P1Y'));

    $nids = _recat_news_get_nodes(variable_get('default_nodes_main', 10), true, 'created', 'DESC', array(
        array('column' => 'created', 'value' => $timestamp_from, 'operator' => '>='),
        array('column' => 'created', 'value' => $date->getTimestamp(), 'operator' => '<'),
    ));

    if (!count($nids)) {
        drupal_not_found();
        drupal_exit();
    }

    $settings = _recat_news_get_settings();
    $teasers = array();

    foreach (node_load_multiple(array_keys($nids)) as $nid => $node) {
        $teasers[] =
            _recat_news_get_teaser_view($node, $settings['image_style'], $settings['date_format'], $settings['max_chars']);
    }

    return array(
        'teasers' => array(
            '#theme' => 'recat_news_listing',
            '#title' => recat_news_year_page_title($year),
            '#teasers' => $teasers,
            '#item_per_line' => 2,
        ),
        'pager' => _recat_core_get_pager_build(),
    );
}

function recat_news_year_page_title($year) {
    return str_replace('!year', $year, _hs_resource_get('recat_news.listing.results_title_year'));
}