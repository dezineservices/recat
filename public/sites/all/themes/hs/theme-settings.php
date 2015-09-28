<?php

function hs_form_system_theme_settings_alter(&$form, $form_state)
{
    $form['hs_settings'] = array(
        '#type' => 'fieldset',
        '#title' => t('Theme settings'),
    );

    $form['hs_settings']['hs_grid_columns'] = array(
        '#type' => 'select',
        '#title' => t('Amount of columns'),
        '#options' => array_combine(range(4, 16), range(4, 16)),
        '#default_value' => theme_get_setting('hs_grid_columns'),
    );

    $form['hs_settings']['hs_debug_grid'] = array(
        '#type' => 'checkbox',
        '#title' => t('Show debug grid overlay'),
        '#default_value' => theme_get_setting('hs_debug_grid'),
    );
}