<?php

function _recat_feedback_block_configure($key) {
    $settings = _recat_feedback_get_settings($key);
    $resource_key = 'recat_feedback';

    $webforms = array();
    foreach (_recat_feedback_get_webforms() as $nid => $webform) {
        $webforms[$nid] = $webform->title;
    }

    $form[$key] = array(
        '#type' => 'fieldset',
        '#title' => t('Feedback content settings'),
        '#tree' => true,
    );

    $form[$key]['webform'] = array(
        '#type' => 'select',
        '#title' => t('Webform'),
        '#description' => t('Webform shown as feedback form'),
        '#required' => true,
        '#options' => $webforms,
        '#default_value' => $settings ? $settings['webform'] : '',
    );

    $form[$key]['content'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="form-item"><label>' . t('Content') . '</label>',
        '#suffix' => '</div>',
        '#markup' => _hs_resource_get($resource_key . '.content', 'markup'),
    );

    $form[$key]['cta'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="form-item"><label>' . t('CTA') . '</label>',
        '#suffix' => '</div>',
        '#markup' => _hs_resource_get($resource_key . '.cta_content'),
    );

    return $form;
}

function _recat_feedback_block_save($form_state, $key) {
    variable_set($key, $form_state[$key]);
}

function _recat_feedback_block_view($key) {
    $settings = _recat_feedback_get_settings($key);
    $resource_key = 'recat_feedback';

    if (!$settings) {
        return;
    }

    $node = node_load($settings['webform']);
    if (!$node) {
        return;
    }

    return array(
        '#theme' => 'recat_feedback',
        '#content' => _hs_resource_get($resource_key . '.content', 'markup'),
        '#cta' => _hs_resource_get($resource_key . '.cta_content', 'plain', null, false),
        '#url' => url(sprintf('node/%d', $node->nid)),
        '#url_overlay' => url(_recat_wf_overlay_get_url($node->nid)),
    );
}