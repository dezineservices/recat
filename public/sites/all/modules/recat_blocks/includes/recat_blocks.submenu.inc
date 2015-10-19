<?php

function _recat_blocks_submenu_configure($key) {
    $settings = _recat_blocks_get_settings($key);
    $form = array();

    $form[$key] = array(
        '#type' => 'fieldset',
        '#title' => t('Submenu content settings'),
        '#tree' => true,
    );

    $form[$key]['menu'] = array(
        '#type' => 'select',
        '#title' => t('Menu name'),
        '#description' => t('The named menu links to return'),
        '#required' => true,
        '#options' => menu_get_menus(),
        '#default_value' => $settings ? $settings['menu'] : '',
    );

    $form[$key]['max_depth'] = array(
        '#type' => 'select',
        '#title' => t('Links depth'),
        '#description' => t('Maximum depth of links to retrieve'),
        '#required' => true,
        '#options' => array_combine(range(1, 5), range(1, 5)),
        '#default_value' => $settings ? $settings['max_depth'] : 1,
    );

    return $form;
}

function _recat_blocks_submenu_save($form_state, $key) {
    variable_set($key, $form_state[$key]);
}

function _recat_blocks_submenu_view($key) {
    $settings = _recat_blocks_get_settings($key);

    $link = _recat_blocks_submenu_get_root_link();
    if (!$link) {
        return;
    }

    $menu_tree = _recat_blocks_submenu_get_tree($link['mlid'], $settings['menu'], $settings['max_depth'] + 1);
    if (empty($menu_tree['below'])) {
        return;
    }

    $links = menu_tree_output($menu_tree['below']);
    if (empty($links)) {
        return;
    }

    return array(
        '#theme' => 'recat_blocks_submenu',
        '#title' => $menu_tree['link']['title'],
        '#links' => $links,
    );
}

function _recat_blocks_submenu_get_root_link() {
    $active_trail = menu_get_active_trail();
    if (!$active_trail || empty($active_trail)) {
        return null;
    }

    $root = array_shift($active_trail);
    if (isset($root['href']) && $root['href'] === '<front>') {
        $root = array_shift($active_trail);
    }

    if (!isset($root['mlid'])) {
        return null;
    }

    return $root;
}

function _recat_blocks_submenu_get_tree($mlid, $menu_name, $max_depth) {
    foreach (menu_tree_all_data($menu_name, null, $max_depth) as $menu_item) {
        if ($menu_item['link']['mlid'] === $mlid) {
            return $menu_item;
        }
    }

    return array();
}