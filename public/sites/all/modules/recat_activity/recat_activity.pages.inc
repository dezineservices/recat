<?php

function recat_activity_taxonomy_page($term) {
    $parents = taxonomy_get_parents($term->tid);
    if (count($parents) === 0) {
        return 'map page';
    }

    if (module_exists('i18n_taxonomy')) {
        $parents = i18n_taxonomy_localize_terms($parents);
        $terms = i18n_taxonomy_localize_terms(array($term));

        $term = array_shift($terms);
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