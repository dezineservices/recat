<?php

function _recat_news_block_configure($key) {
    $form[$key] = array(
        '#type' => 'fieldset',
        '#title' => t('News listing content settings'),
        '#tree' => true,
    );

    $form[$key]['settings_page'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="form-item">',
        '#suffix' => '</div>',
        '#markup' => t('News listing settings can be managed on the !url page.', array(
            '!url' => l('News settings', 'admin/config/recat/news'),
        )),
    );

    return $form;
}

function _recat_news_block_save($form_state, $key) {
    // nothing to save
}


function _recat_news_block_view($key) {
    $settings = _recat_news_get_settings();
    $nids = _recat_news_get_nodes($settings['items_per_page'], true);

    if (!$nids) {
        return _hs_resource_get('recat_news.no_posts', 'markup');
    }

    $nodes = node_load_multiple(array_keys($nids));
    if (!$nodes) {
        return _hs_resource_get('recat_news.no_posts', 'markup');
    }

    $teasers = array();
    foreach ($nodes as $nid => $node) {
        $teasers[] =
            _recat_news_get_teaser_view($node, $settings['image_style'], $settings['date_format'], $settings['max_chars']);
    }

    return array(
        'teasers' => array(
            '#theme' => 'recat_news_listing',
            '#teasers' => $teasers
        ),
        'pager' => _recat_news_get_pager_build(),
    );
}

function _recat_news_block_latest_configure($key) {
    $settings = _recat_news_get_settings($key);
    $form = array();

    $form[$key] = array(
        '#type' => 'fieldset',
        '#title' => t('Latest news content settings'),
        '#tree' => true,
    );

    $form[$key]['amount'] = array(
        '#type' => 'select',
        '#title' => t('Amount'),
        '#description' => t('The amount of latest news articles to show'),
        '#required' => true,
        '#options' => array_combine(range(1, 9), range(1, 9)),
        '#default_value' => $settings ? $settings['amount'] : 1,
    );

    return $form;
}

function _recat_news_block_latest_save($form_state, $key) {
    variable_set($key, $form_state[$key]);
}

function _recat_news_block_latest_view($key) {
    $settings = array_merge(
        _recat_news_get_settings(), _recat_news_get_settings($key));

    $nids = _recat_news_get_nodes($settings['amount']);
    if (!$nids) {
        return;
    }

    $nodes = node_load_multiple(array_keys($nids));
    if (!$nodes) {
        return;
    }

    $teasers = array();
    foreach ($nodes as $nid => $node) {
        $teasers[] =
            _recat_news_get_teaser_view($node, $settings['image_style'], $settings['date_format'], $settings['max_chars']);
    }

    return theme('recat_news_latest_news', array(
        'teasers' => $teasers,
    ));
}

function _recat_news_block_tags_configure($key) {

}

function _recat_news_block_tags_save($form_state, $key) {
    // nothing to save
}

function _recat_news_block_tags_view($key) {
    return theme('recat_news_tags', array(
        'tags' => _recat_news_get_tags(),
    ));
}

function _recat_news_block_categories_configure($key) {

}

function _recat_news_block_categories_save($form_state, $key) {
    // nothing to save
}

function _recat_news_block_categories_view($key) {
    $categories = array();
    foreach (_recat_news_get_categories() as $category) {
        $categories[] = array(
            'term' => $category,
            'count' => _recat_news_get_count_by_term($category),
        );
    }

    return theme('recat_news_categories', array(
        'categories' => $categories,
    ));
}