<?php

function _recat_blocks_facts_configure($key) {
    $form = array();

    $settings = _recat_blocks_get_settings($key);
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

    $terms = array();
    foreach (_recat_activity_get_terms() as $term) {
        $terms[$term->tid] = $term->name;
    }

    $form[$key]['conference_docs_tid'] = array(
        '#type' => 'select',
        '#title' => t('Conference docs taxonomy'),
        '#required' => true,
        '#options' => $terms,
        '#default_value' => $settings ? $settings['conference_docs_tid'] : '',
    );

    $form[$key]['project_files_tid'] = array(
        '#type' => 'select',
        '#title' => t('Project files taxonomy'),
        '#required' => true,
        '#options' => $terms,
        '#default_value' => $settings ? $settings['project_files_tid'] : '',
    );
        
    return $form;
}

function _recat_blocks_facts_save($form_state, $key) {
    variable_set($key, $form_state[$key]);
}

function _recat_blocks_facts_view($key) {
    $facts = array();

    $settings = _recat_blocks_get_settings($key);
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
                $number = _recat_blocks_facts_get_node_count_for_tid($settings['conference_docs_tid']);
                break;
            case 'project_files':
                $number = _recat_blocks_facts_get_node_count_for_tid($settings['project_files_tid']);
                break;
        }

        $facts[] = array(
            'class' => $fact_key,
            'label' => _hs_resource_get($resource_key . '.title'),
            'number' => _recat_blocks_facts_format_number($number),
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

function _recat_blocks_facts_get_node_count_for_tid($tid) {
    $tids = array();
    foreach (_recat_activity_get_terms($tid) as $term) {
        $tids[] = $term->tid;
    }

    if (empty($tids)) {
        return 0;
    }

    return _recat_activity_get_nodes_count($tids);
}

function _recat_blocks_facts_format_number($input) {
    $suffixes = array('', 'k', 'm', 'g', 't');
    $suffixIndex = 0;

    while(abs($input) >= 1000 && $suffixIndex < sizeof($suffixes)) {
        $suffixIndex++;
        $input /= 1000;
    }

    return (
        $input > 0
            ? floor($input * 1000) / 1000
            : ceil($input * 1000) / 1000
    ) . $suffixes[$suffixIndex];
}