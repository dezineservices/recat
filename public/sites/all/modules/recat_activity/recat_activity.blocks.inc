<?php

function _recat_activity_block_configure($key) {
    $form[$key] = array(
        '#type' => 'fieldset',
        '#title' => t('Activity content settings'),
        '#tree' => true,
    );

    $form[$key]['settings_page'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="form-item">',
        '#suffix' => '</div>',
        '#markup' => t('Activity settings can be managed on the !url page.', array(
            '!url' => l('Activity settings', 'admin/config/recat/activity'),
        )),
    );

    return $form;
}

function _recat_activity_block_view() {
    $vocabulary = taxonomy_vocabulary_machine_name_load(RECAT_ACTIVITY_VOCABULARY_NAME);
    if (!$vocabulary) {
        return;
    }

    $terms = _recat_activity_get_terms();
    if (!$terms) {
        return;
    }

    $active_term = _recat_activity_get_active_term();
    $parents = $active_term ? taxonomy_get_parents($active_term->tid) : array();

    if (count($parents)) {
        $active_term = reset($parents);
    }

    foreach ($terms as $term) {
        $term->css_list_classes = array();
        $term->css_classes = array();

        if ($active_term && !count($parents) && $term->tid === $active_term->tid) {
            $term->css_classes[] = 'active';
        }

        if (count($parents) && $term->tid === $active_term->tid) {
            $term->css_list_classes[] = 'active-trail';
            $term->css_list_classes[] = 'expanded';

            $term->terms = _recat_activity_block_assign_count(
                _recat_activity_get_terms($term->tid), _recat_activity_get_active_term());
        }
    }

    return array(
        '#theme' => 'recat_activity_sidebar',
        '#terms' => $terms,
    );
}

function _recat_activity_block_assign_count($terms, $active_term) {
    foreach ($terms as $term) {
        $term->css_classes = array();

        if ($active_term && $term->tid === $active_term->tid) {
            $term->css_classes[] = 'active';
        }

        $term->activities_count = _recat_activity_get_nodes_count($term->tid);
    }

    return $terms;
}
