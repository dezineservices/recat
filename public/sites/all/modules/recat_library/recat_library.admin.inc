<?php

function recat_library_admin($form, &$form_state) {
    $settings = _recat_library_get_settings();
    $key = 'recat_library';

    $form[$key] = array(
        '#type' => 'container',
        '#tree' => true,
    );

    $webforms = array();
    foreach (_recat_library_admin_get_webforms() as $nid => $webform) {
        $webforms[$nid] = $webform->title;
    }

    $form[$key]['webform'] = array(
        '#type' => 'select',
        '#title' => t('Webform'),
        '#description' => t('Webform shown for private file request'),
        '#required' => true,
        '#options' => $webforms,
        '#default_value' => $settings ? $settings['webform'] : '',
    );

    $items_per_page = array();
    for ($i = 1; $i < 102; $i += 5) {
        $items_per_page[$i - ($i === 1 ? 0 : 1)] = $i - ($i === 1 ? 0 : 1);
    }

    $form[$key]['items_per_page'] = array(
        '#type' => 'select',
        '#title' => t('Items per page'),
        '#description' => t('The amount of files to show per page'),
        '#required' => true,
        '#options' => $items_per_page,
        '#default_value' => $settings ? $settings['items_per_page'] : 1,
    );

    $form[$key]['date_format'] = array(
        '#type' => 'select',
        '#title' => t('Date format'),
        '#required' => true,
        '#options' => array_map(function($item) {
            return $date_formats[$item['type']] = format_date(time(), $item['type']);
        }, system_get_date_types()),
        '#default_value' => $settings ? $settings['date_format'] : 1,
    );

    $tags_query = array('OR' => 'OR', 'AND' => 'AND');
    $form[$key]['tags_query'] = array(
        '#type' => 'select',
        '#title' => t('Tags query'),
        '#description' => t('Query executed on the <em>tags</em> clausule'),
        '#required' => true,
        '#options' => $tags_query,
        '#default_value' => $settings ? $settings['tags_query'] : 1,
    );

    $form[$key]['document_title'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="form-item"><label>' . t('Document title table heading') . '</label>',
        '#suffix' => '</div>',
        '#markup' => _hs_resource_get('recat_library.table.document_title'),
    );

    $form[$key]['document_tags'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="form-item"><label>' . t('Document tags table heading') . '</label>',
        '#suffix' => '</div>',
        '#markup' => _hs_resource_get('recat_library.table.tags'),
    );

    $form[$key]['document_date'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="form-item"><label>' . t('Document date table heading') . '</label>',
        '#suffix' => '</div>',
        '#markup' => _hs_resource_get('recat_library.table.date'),
    );

    $form[$key]['document_date'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="form-item"><label>' . t('No results message') . '</label>',
        '#suffix' => '</div>',
        '#markup' => _hs_resource_get('recat_library.no_results_message'),
    );

    return system_settings_form($form);
}

function _recat_library_admin_get_webforms() {
    global $language;

    $query = db_select('node', 'n');

    $query
        ->fields('n', array('nid', 'title'))
        ->condition('status', 1)
        ->condition('language', $language->language)
        ->condition('type', 'webform')
        ->orderBy('title')
        ->addTag('node_access');

    return $query->execute()->fetchAllAssoc('nid');
}