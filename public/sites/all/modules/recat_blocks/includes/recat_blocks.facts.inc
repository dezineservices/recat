<?php

function _recat_blocks_facts_configure($key) {
    $form = array();
    $facts = _recat_blocks_facts_get_fact_keys();

    $form[$key] = array(
        '#type' => 'fieldset',
        '#title' => t('Facts content settings'),
        '#tree' => true,
    );

    foreach ($facts as $fact) {
        $resource_key = sprintf('recat_blocks.facts.%s', $fact);

        $form[$key][$fact] = array(
            '#type' => 'markup',
            '#prefix' => '<div class="form-item">',
            '#suffix' => '</div>',
            '#markup' => _hs_resource_get($resource_key . '.title'),
        );
    }
        
    return $form;
}

function _recat_blocks_facts_save($form_state, $key) {
    // nothing to save
}

function _recat_blocks_facts_view($key) {
    $facts = array();
    $fact_keys = _recat_blocks_facts_get_fact_keys();

    foreach ($fact_keys as $fact_key) {
        $resource_key = sprintf('recat_blocks.facts.%s', $fact_key);

        $number = 0;
        switch ($fact_key) {
            case 'unique_visits':
                $number = _recat_blocks_facts_get_unique_visitors();
                break;
            case 'news_posts':
                $number = _recat_blocks_facts_get_news_posts_count();
                break;
            case 'conference_docs':
                break;
            case 'project_files':
                break;
        }

        $facts[] = array(
            'class' => $fact_key,
            'label' => _hs_resource_get($resource_key . '.title'),
            'number' => $number,
        );
    }

    return theme('recat_blocks_facts', array('facts' => $facts));
}

function _recat_blocks_facts_get_fact_keys() {
    return array('unique_visits', 'news_posts', 'conference_docs', 'project_files');
}

function _recat_blocks_facts_get_unique_visitors() {
    return db_query('SELECT COUNT(DISTINCT visitors_ip) FROM {visitors}')->fetchField();
}

function _recat_blocks_facts_get_news_posts_count() {
    global $language;

    $query = db_select('node', 'n');
    $query
        ->fields('n', array('nid'))
        ->condition('status', 1)
        ->condition('language', $language->language)
        ->condition('type', 'news')
        ->addTag('node_access');

    return $query->execute()->rowCount();
}