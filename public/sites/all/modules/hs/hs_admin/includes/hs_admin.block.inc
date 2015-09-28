<?php

function hs_admin_block_form_alter(&$form, &$form_state, $form_id)
{
    // Remove default Drupal blocks from the Blocks structure page
    if ($form_id == 'block_admin_display_form') {
        _hs_admin_block_admin_form_alter($form, $form_state);
    }

    // Copy the actions bar on top for on all node edit pages
    $node_edit_form_suffix = '_node_form';
    if ($node_edit_form_suffix == substr($form_id, -strlen($node_edit_form_suffix))) {
        _hs_admin_block_node_form_alter($form, $form_state);
    }
}

function _hs_admin_block_admin_form_alter(&$form, &$form_state)
{
    $hidden_blocks = array(
        'system_help', 
        'menu_devel', 
        'devel_execute_php', 
        'system_main-menu', 
        'system_management',
        'system_navigation',
        'system_powered-by',
        'node_recent',
        'shortcut_shortcuts',
        'devel_switch_user',
        'node_syndicate',
        'user_login',
        'system_user-menu',
        'user_new',
        'user_online',
        'menu_menu-footer-menu',
    );

    $form['blocks'] = array_filter($form['blocks'], function($item) use ($hidden_blocks) {
        return !in_array(sprintf(
            '%s_%s', $item['module']['#value'], $item['delta']['#value']), $hidden_blocks);
    });
}

function _hs_admin_block_node_form_alter(&$form, &$form_state)
{
    $actions_top = $form['actions'];
    $actions_top['#weight'] = -9999;

    $form['actions_top'] = $actions_top;
}

/**
 * Move the implementation of hs_admin_block_form_alter to the bottom of the list.
 * This allows other modules to be
 */
function hs_admin_block_submodule_implements_alter(&$implementations, $hook)
{
    if ($hook == 'form_alter' && isset($implementations['hs_admin_block'])) {
        $submodule = $implementations['hs_admin_block'];
        unset($implementations['hs_admin_block']);
        $implementations['hs_admin_block'] = $submodule;
    }
}
