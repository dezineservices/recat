<?php

function _recat_blocks_headliner_configure($key) {
    $settings = _recat_blocks_get_settings($key);
    $form = array();

    $form[$key] = array(
        '#type' => 'fieldset',
        '#title' => t('Headliner content settings'),
        '#tree' => true,
    );

    $form[$key]['title'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="form-item"><label>' . t('Title') . '</label>',
        '#suffix' => '</div>',
        '#markup' => _hs_resource_get('recat_blocks.headliner.title'),
    );

    $form[$key]['content'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="form-item"><label>' . t('Content') . '</label>',
        '#suffix' => '</div>',
        '#markup' => _hs_resource_get('recat_blocks.headliner.content'),
    );
    
    $form[$key]['image'] = array(
        '#type' => 'managed_file',
        '#title' => t('Image'),
        '#required' => true,
        '#upload_location' => 'public://headliner/',
        '#default_value' => $settings ? $settings['image'] : 0,
    );

    $form[$key]['cta'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="form-item"><label>' . t('CTA') . '</label>',
        '#suffix' => '</div>',
        '#markup' => _hs_resource_get('recat_blocks.headliner.cta_content'),
    );

    $form[$key]['url'] = array(
        '#type' => 'textfield',
        '#title' => t('Url'),
        '#description' => t('Url set for the CTA button'),
        '#required' => true,
        '#default_value' => $settings ? $settings['url'] : '',
    );
        
    return $form;
}

function _recat_blocks_headliner_save($form_state, $key) {
    $settings = _recat_blocks_get_settings($key);
    $form_values = $form_state[$key];
    
    _recat_blocks_handle_managed_file(
        $settings ? $settings['image'] : 0, $form_values['image']);
    
    variable_set($key, $form_values);
}

function _recat_blocks_headliner_view($key) {
    global $language;
    
    $settings = _recat_blocks_get_settings($key);
    if (!$settings) {
        return false;
    }
    
    $image = file_load($settings['image']);

    $path = $settings['url'];
    $paths = translation_path_get_translations($path);

    if (!empty($paths) && isset($paths[$language->language])) {
        $path = $paths[$language->language];
    }

    return theme('recat_blocks_headliner', array(
        'image_url' => file_create_url($image->uri),
        'url' => $path,
        'title' => _hs_resource_get('recat_blocks.headliner.title', 'plain', null, false),
        'content' => _hs_resource_get('recat_blocks.headliner.content', 'plain', null, false),
        'cta' => _hs_resource_get('recat_blocks.headliner.cta_content', 'plain', null, false),
    ));
}