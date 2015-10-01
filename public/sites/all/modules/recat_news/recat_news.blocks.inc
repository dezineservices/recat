<?php

function _recat_news_block_latest_configure($key) {
    $settings = _recat_news_get_settings($key);
    $form = array();

    $form[$key] = array(
        '#type' => 'fieldset',
        '#title' => t('Latest news content settings'),
        '#tree' => true,
    );

    $form[$key]['amount'] = array(
        '#type' => 'select',
        '#title' => t('Amount'),
        '#description' => t('The amount of latest news articles to show'),
        '#required' => true,
        '#options' => array_combine(range(1, 9), range(1, 9)),
        '#default_value' => $settings ? $settings['amount'] : 1,
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
        '#description' => t('Truncates the teaser content to a number of characters'),
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

    return $form;
}

function _recat_news_block_latest_save($form_state, $key) {
    variable_set($key, $form_state[$key]);
}

function _recat_news_block_latest_view($key) {
    $settings = _recat_news_get_settings($key);
    $nids = _recat_news_get_nodes($settings['amount']);

    if (!$nids) {
        return;
    }

    $nodes = node_load_multiple(array_keys($nids));
    if (!$nodes) {
        return;
    }

    $teasers = array();

    foreach ($nodes as $nid => $node) {
        $node_wrapper = entity_metadata_wrapper('node', $node);

        $body = $node_wrapper->body->value();
        $image = $node_wrapper->field_image->value();
        $content = filter_xss($body['summary'] ? $body['summary'] : $body['value'], array());

        $teasers[] = array(
            '#theme' => 'recat_news_teaser',
            '#image' => theme_image_style(array(
                'style_name' => $settings['image_style'],
                'path' => $image['uri'],
                'width' => $image['width'],
                'height' => $image['height'],
                'title' => $image['title'],
                'alt' => $image['alt'],
                'attributes' => array(
                    'class' => array('image'),
                ),
            )),
            '#post_date' => format_date($node->created, $settings['date_format']),
            '#title' => $node_wrapper->title->value(),
            '#content' => truncate_utf8($content, $settings['max_chars'], true, true),
            '#cta' => _hs_resource_get('recat_news.teaser.read_more', 'plain', null, false),
            '#url' => node_uri($node),
        );
    }

    return theme('recat_news_latest_news', array(
        'teasers' => $teasers,
    ));
}