<?php

function _recat_blocks_quick_access_configure($key) {
    $settings = _recat_blocks_get_settings($key);
    $count = _recat_blocks_quick_access_get_amount_boxes();

    $form = array();
    $form[$key] = array(
        '#type' => 'fieldset',
        '#title' => t('Quick access content settings'),
        '#tree' => true,
    );

    for ($i = 0; $i < $count; $i++) {
        $item_key = sprintf('box_%d', $i);
        $resource_key = sprintf('recat_blocks.quick_access.%d', $i + 1);

        $setting = isset($settings[$item_key]) ? $settings[$item_key] : null;
        $is_enabled = $setting['enabled'];

        $form[$key][$item_key] = array(
            '#type' => 'fieldset',
            '#title' => t('Quick access box !number [@enabled]', array(
                '!number' => $i + 1,
                '@enabled' => $is_enabled ? t('enabled') : t('not enabled'),
            )),
            '#collapsible' => true,
            '#collapsed' => true,
        );

        $form[$key][$item_key]['enabled'] = array(
            '#type' => 'checkbox',
            '#title' => t('Enabled?'),
            '#default_value' => $setting['enabled'] ? $setting['enabled'] : false,
        );

        $form[$key][$item_key]['title'] = array(
            '#type' => 'markup',
            '#prefix' => '<div class="form-item"><label>' . t('Title') . '</label>',
            '#suffix' => '</div>',
            '#markup' => _hs_resource_get($resource_key . '.title'),
        );

        $form[$key][$item_key]['content'] = array(
            '#type' => 'markup',
            '#prefix' => '<div class="form-item"><label>' . t('Content') . '</label>',
            '#suffix' => '</div>',
            '#markup' => _hs_resource_get($resource_key . '.content'),
        );

        $form[$key][$item_key]['more_info'] = array(
            '#type' => 'markup',
            '#prefix' => '<div class="form-item"><label>' . t('More info') . '</label>',
            '#suffix' => '</div>',
            '#markup' => _hs_resource_get($resource_key . '.more_info'),
        );

        $form[$key][$item_key]['icon'] = array(
            '#type' => 'textfield',
            '#title' => t('Icon'),
            '#description' => t('CSS class used to determine which icon to show'),
            '#default_value' => $setting['icon'] ? $setting['icon'] : '',
        );

        $form[$key][$item_key]['url'] = array(
            '#type' => 'textfield',
            '#title' => t('Url'),
            '#description' => t('Url set for the more info link'),
            '#default_value' => $setting['url'] ? $setting['url'] : '',
        );
    }
        
    return $form;
}

function _recat_blocks_quick_access_save($form_state, $key) {
    variable_set($key, $form_state[$key]);
}

function _recat_blocks_quick_access_view($key) {
    global $language;
    
    $settings = _recat_blocks_get_settings($key);
    if (!$settings) {
        return false;
    }

    $count = _recat_blocks_quick_access_get_amount_boxes();
    $boxes = array();

    for ($i = 0; $i < $count; $i++) {
        $item_key = sprintf('box_%d', $i);
        $resource_key = sprintf('recat_blocks.quick_access.%d', $i + 1);

        $setting = isset($settings[$item_key]) ? $settings[$item_key] : null;
        if (!$setting || !$setting['enabled']) {
            continue;
        }

        $path = $setting['url'];
        $paths = translation_path_get_translations($path);

        if (!empty($paths) && isset($paths[$language->language])) {
            $path = $paths[$language->language];
        }

        $boxes[] = array(
            '#theme' => 'recat_blocks_quick_access_box',
            '#title' => _hs_resource_get($resource_key . '.title', 'plain', null, false),
            '#content' => _hs_resource_get($resource_key . '.content', 'plain', null, false),
            '#icon' => $setting['icon'],
            '#url' => $path,
            '#more_info' => _hs_resource_get($resource_key . '.more_info', 'plain', null, false),
            '#is_last' => false,
        );
    }

    if (count($boxes)) {
        $boxes[count($boxes) - 1]['#is_last'] = true;
    }

    return theme('recat_blocks_quick_access', array('boxes' => $boxes));
}

function _recat_blocks_quick_access_get_amount_boxes() {
    return 9;
}