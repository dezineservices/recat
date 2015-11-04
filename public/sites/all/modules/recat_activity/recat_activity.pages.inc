<?php

function recat_activity_taxonomy_page($term) {
    $parents = taxonomy_get_parents($term->tid);
    if (module_exists('i18n_taxonomy')) {
        $parents = i18n_taxonomy_localize_terms($parents);
        $terms = i18n_taxonomy_localize_terms(array($term));

        $term = array_shift($terms);
    }

    if (count($parents) === 0) {
        return recat_activity_taxonomy_page_map($term);
    }

    $parent = array_pop($parents);

    return array(
        'activity' => array(
            '#theme' => 'recat_activity_overview',
            '#title' => $parent->name,
            '#subtitle' => $term->name,
            '#content' => check_markup($term->description, $term->format),
        ),
        'table' => array(
            '#theme' => 'recat_activity_table',
            '#term' => $parent,
        ),
        '#attached' => array(
            'js' => array(
                array(
                    'data' => array('recatActivity' => array(
                        'serviceUrl' => url(_recat_activity_get_url(), array('absolute' => true)),
                        'tid' => $term->tid,
                    )),
                    'type' => 'setting',
                ), array(
                    'data' => drupal_get_path('module', 'recat_activity') . '/js/recat_activity.min.js',
                    'type' => 'file',
                )
            ),
            'library' => array(
                array('recat_core', 'recat_core_angularjs'),
            ),
        ),
    );
}

function recat_activity_taxonomy_page_map($parent_term) {
    $parent_term->is_map_page = true;
    $terms = array();

    foreach (_recat_activity_get_terms($parent_term->tid) as $term) {
        $term = taxonomy_term_load($term->tid);
        $term_wrapper = entity_metadata_wrapper('taxonomy_term', $term);

        $terms[] = array(
            'tid' => $term->tid,
            'url' => url(sprintf('taxonomy/term/%d', $term->tid)),
            'countryCode' => strtoupper($term_wrapper->field_country_code->value()),
            'count' => _recat_activity_get_nodes_count(array($term->tid)),
        );
    }

    return array(
        'activity' => array(
            '#theme' => 'recat_activity_map',
            '#title' => $parent_term->name,
            '#content' => check_markup($parent_term->description, $parent_term->format),
        ),
        '#attached' => array(
            'js' => array(
                array(
                    'data' => array('recatActivityMap' => array(
                        'terms' => $terms,
                    )),
                    'type' => 'setting',
                ),
            ),
            'library' => array(
                array('recat_activity', 'recat_activity_map'),
            ),
        ),
    );
}