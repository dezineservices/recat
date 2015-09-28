<?php

/**
 * Change the visual representation of items, based on their publish status.
 *
 * Implements hook_form_alter().
 */
function hs_admin_status_color_form_alter(&$form, &$form_state, $form_id)
{
    switch ($form_id) {
        case 'node_admin_content':
        case 'hs_admin_content_search_form':
            _hs_admin_status_color_node_admin_form_alter($form, $form_state);
            break;
    }
}

/**
 * Change visual representation of node rows in Administration > Content.
 */
function _hs_admin_status_color_node_admin_form_alter(&$form, &$form_state)
{
    $form['#attached']['css'][] = drupal_get_path('module', 'hs_admin') . '/css/hs_admin.status_color.css';
    if (!isset($form['admin']['nodes']['#options'])) {
        return;
    }

    foreach ($form['admin']['nodes']['#options'] as $nid => &$node) {
        $published = t('published') === $node['status'];
        $node['#attributes']['class'][] = $published ? 'published' : 'not-published';
    }
}
