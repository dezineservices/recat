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
    $terms = _recat_activity_get_terms();
    if (!$terms) {
        return;
    }

    if (module_exists('i18n_taxonomy')) {
        $terms = i18n_taxonomy_localize_terms($terms);
    }

    $build = array();
    foreach ($terms as $term) {
        $build[] = theme('recat_activity_overview', array(
            'title' => $term->name,
            'content' => check_markup($term->description, $term->format),
        ));
    }

    $block = block_load('recat_activity', 'recat_activity_sidebar');

    return array(
        '#theme' => 'recat_activity_landing',
        '#terms' => $build,
        '#blocks' => _block_get_renderable_array(
            _block_render_blocks(array($block))
        ),
    );

    return 'test';
}

function _recat_activity_block_sidebar_view() {
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

            $term->terms = _recat_activity_block_sidebar_assign_count(
                _recat_activity_get_terms($term->tid), _recat_activity_get_active_term());
        }
    }

    return array(
        '#theme' => 'recat_activity_sidebar',
        '#terms' => $terms,
    );
}

function _recat_activity_block_sidebar_assign_count($terms, $active_term) {
    foreach ($terms as $term) {
        $term->css_classes = array();

        if ($active_term && $term->tid === $active_term->tid) {
            $term->css_classes[] = 'active';
        }

        $term->activities_count = _recat_activity_get_nodes_count(array($term->tid));
    }

    return $terms;
}
