<?php

function _hs_enhancements_field_settings_form(&$form, $form_state)
{
    $field = $form['#field'];

    $form['field']['settings']['template'] = array(
        '#type' => 'select',
        '#title' => t('Template'),
        '#options' => _hs_enhancements_field_get_templates(),
        '#default_value' => $field['settings']['template'],
        '#description' => t('Defines the HTML output used for this field and adds this as a theme hook suggestion')
    );
}

function _hs_enhancements_field_get_default_template()
{
    $templates = _hs_enhancements_field_get_templates(false);
    return $templates[1];
}

function _hs_enhancements_field_get_templates($forForm = true)
{
    $templates = array(
        'empty' => t('Empty'),
        'minimal' => t('Minimal'),
        'full' => t('Full'),
    );

    return $forForm ? $templates : array_keys($templates);
}