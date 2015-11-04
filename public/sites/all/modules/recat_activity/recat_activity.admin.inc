<?php

function recat_activity_admin($form, &$form_state) {
    $settings = _recat_activity_get_settings();
    $key = 'recat_activity';

    $form[$key] = array(
        '#type' => 'container',
        '#tree' => true,
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

    $vocabulary = taxonomy_vocabulary_machine_name_load(RECAT_ACTIVITY_VOCABULARY_NAME);
    $terms = taxonomy_get_tree($vocabulary->vid, 0, 1);

    foreach ($terms as $tid => $term) {
        $form[$key]['view_all_cta_' . $term->tid] = array(
            '#type' => 'markup',
            '#prefix' => '<div class="form-item"><label>' . t('!name view all CTA button', array('!name' => $term->name)) . '</label>',
            '#suffix' => '</div>',
            '#markup' => _hs_resource_get('recat_activity.view.all.cta.' . $term->tid),
        );
    }

    foreach ($terms as $tid => $term) {
        $form[$key]['name_' . $term->tid] = array(
            '#type' => 'markup',
            '#prefix' => '<div class="form-item"><label>' . t('!name name table heading', array('!name' => $term->name)) . '</label>',
            '#suffix' => '</div>',
            '#markup' => _hs_resource_get('recat_activity.table.name.' . $term->tid),
        );
    }

    $form[$key]['status'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="form-item"><label>' . t('Status table heading') . '</label>',
        '#suffix' => '</div>',
        '#markup' => _hs_resource_get('recat_activity.table.status'),
    );

    $form[$key]['continent'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="form-item"><label>' . t('Continent table heading') . '</label>',
        '#suffix' => '</div>',
        '#markup' => _hs_resource_get('recat_activity.table.continent'),
    );

    $form[$key]['country'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="form-item"><label>' . t('Country table heading') . '</label>',
        '#suffix' => '</div>',
        '#markup' => _hs_resource_get('recat_activity.table.country'),
    );

    $form[$key]['no_message'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="form-item"><label>' . t('No results message') . '</label>',
        '#suffix' => '</div>',
        '#markup' => _hs_resource_get('recat_activity.no_results_message'),
    );

    return system_settings_form($form);
}