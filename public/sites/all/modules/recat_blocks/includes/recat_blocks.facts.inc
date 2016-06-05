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

    $date_formats = array();
    foreach (system_get_date_types() as $date_type) {
        $date_formats[$date_type['type']] = $date_type['title'];
    }

    $form[$key]['date_format'] = array(
        '#type' => 'select',
        '#title' => t('Date format'),
        '#required' => true,
        '#options' => $date_formats,
        '#default_value' => $settings ? $settings['date_format'] : '',
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
            case 'total_projects':
                $number = _recat_blocks_facts_get_project_posts_count();
                break;
            case 'latest_node_update':
                $number = _recat_blocks_facts_get_latest_node_update();
                break;
        }

        $facts[] = array(
            'class' => $fact_key,
            'label' => _hs_resource_get($resource_key . '.title'),
            'number' => $fact_key === 'latest_node_update'
                ? _recat_blocks_facts_format_date($number, isset($settings['date_format']) ? $settings['date_format'] : '')
                : _recat_blocks_facts_format_number($number),
        );
    }

    return theme('recat_blocks_facts', array('facts' => $facts));
}

function _recat_blocks_facts_get_fact_keys() {
    return array('unique_visits', 'total_projects', 'latest_node_update');
}

function _recat_blocks_facts_get_unique_visitors() {
    return db_query('SELECT COUNT(DISTINCT visitors_ip) FROM {visitors}')->fetchField();
}

function _recat_blocks_facts_get_project_posts_count() {
    global $language;

    $query = db_select('node', 'n');
    $query
        ->fields('n', array('nid'))
        ->condition('status', 1)
        ->condition('language', $language->language)
        ->condition('type', 'activity')
        ->addTag('node_access');

    return $query->execute()->rowCount();
}

function _recat_blocks_facts_get_latest_node_update() {
    global $language;

    $query = db_select('node', 'n');
    $query
        ->fields('n', array('changed'))
        ->condition('status', 1)
        ->condition('language', $language->language)
        ->orderBy('changed', 'DESC')
        ->range(0, 1)
        ->addTag('node_access');

    $latest_node = $query->execute()->fetch();
    if (!$latest_node) {
        return null;
    }

    return $latest_node->changed;
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

function _recat_blocks_facts_format_date($timestamp, $date_format = 'medium') {
    return format_date($timestamp, $date_format);
}