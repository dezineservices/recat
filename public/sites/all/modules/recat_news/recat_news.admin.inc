<?php

function recat_news_admin($form, &$form_state) {
    $settings = _recat_news_get_settings();
    $key = 'recat_news';

    $form[$key] = array(
        '#type' => 'container',
        '#tree' => true,
    );

    $form[$key]['items_per_page'] = array(
        '#type' => 'select',
        '#title' => t('Items per page'),
        '#description' => t('The amount of news articles to show per page'),
        '#required' => true,
        '#options' => array_combine(range(1, 10), range(1, 10)),
        '#default_value' => $settings ? $settings['items_per_page'] : 1,
    );

    $form[$key]['max_chars'] = array(
        '#type' => 'select',
        '#title' => t('Truncate'),
        '#description' => t('Truncates the teaser content to a number of characters'),
        '#required' => true,
        '#options' => array_combine(range(50, 250), range(50, 250)),
        '#default_value' => $settings ? $settings['max_chars'] : 1,
    );

    $form[$key]['date_format'] = array(
        '#type' => 'select',
        '#title' => t('Date format'),
        '#required' => true,
        '#options' => array_map(function($item) {
            return $date_formats[$item['type']] = format_date(time(), $item['type']);
        }, system_get_date_types()),
        '#default_value' => $settings ? $settings['date_format'] : 1,
    );

    $form[$key]['image_style'] = array(
        '#type' => 'select',
        '#required' => true,
        '#title' => t('Image style'),
        '#options' => array_map(function($item) {
            return $image_styles[$item['name']] = $item['label'];
        }, image_styles()),
        '#description' => t('Image style that will be used for resizing and cropping the teaser image'),
        '#default_value' => $settings ? $settings['image_style'] : '',
    );

    $form[$key]['cta_button'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="form-item"><label>' . t('CTA') . '</label>',
        '#suffix' => '</div>',
        '#markup' => _hs_resource_get('recat_news.teaser.read_more'),
    );

    return system_settings_form($form);
}