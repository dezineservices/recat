<?php

function recat_preprocess_html(&$variables) {
    _recat_preprocess_html_fonts();

}

function recat_preprocess_page(&$variables) {
    $variables['has_main_content'] = true;

    $variables['site_name_abbr'] = _hs_resource_get('global.site_name_abbr', 'plain', null, false);
    $variables['footer_copyright'] = str_replace(
        '{{YEAR}}', date('Y'), _hs_resource_get('global.footer_copyright', 'markup'));

    if (isset($variables['page']['content']['system_main']['default_message'])) {
        unset($variables['page']['content']['system_main']['default_message']);
    }

    $messages = theme('status_messages');
    if (strlen($messages)) {
        $variables['page']['content']['system_main']['messages'] = array(
            '#weight' => -99,
            '#markup' => $messages,
        );
    }

    _recat_preprocess_page_title($variables);
    _recat_preprocess_page_tabs($variables);
    _recat_preprocess_page_main_content($variables);
    _recat_preprocess_page_block_reference($variables);
    _recat_preprocess_page_news($variables);
    _recat_preprocess_page_search($variables);
}

function recat_preprocess_node(&$variables) {
    if ($variables['type'] === 'news') {
        $variables['classes_array'][] = 'panel panel-node';
        $variables['title_prefix'][] = array(
            '#markup' => sprintf('<h1 class="h3 node-title">%s</h1>', $variables['title']),
        );
    }
}

function recat_preprocess_block(&$variables) {
    $variables['block_html_id'] = str_replace('BlocksRecatBlocks', '', $variables['block_html_id']);
    $variables['block_html_id'] = str_replace('RecatNews', '', $variables['block_html_id']);

    switch ($variables['block_html_id']) {
        case 'recatNews':
            $variables['classes_array'][] = 'block-neutral';
            break;
        case 'recatNewsTags':
        case 'recatNewsCategories':
            $variables['theme_hook_suggestions'][] = 'block__sidebar_panel';
            $variables['title_attributes_array']['class'] = array('title');
        case 'recatSubmenu':
            $variables['classes_array'][] = 'panel-sidebar';
            break;

    }
}

function recat_preprocess_search_results(&$variables) {
    $pager = _recat_core_get_pager_build();
    $variables['pager'] = drupal_render($pager);
}

function recat_form_alter(&$form, &$form_state, $form_id) {
    switch ($form_id) {
        case 'search_block_form':
            _recat_search_form_alter($form);
            break;
    }
}

function recat_pager_first(&$variables) {
    if ($variables['text'] === 'first') {
        return;
    }

    return theme_pager_first($variables);
}

function recat_pager_last(&$variables) {
    if ($variables['text'] === 'last') {
        return;
    }

    return theme_pager_last($variables);
}

function _recat_preprocess_html_fonts() {
    drupal_add_css('//fonts.googleapis.com/css?family=Roboto+Condensed|Roboto:400,300,500,700', array(
        'type' => 'external',
        'weight' => -100,
    ));
}

function _recat_preprocess_page_title(&$variables) {
    $active_trail = menu_get_active_trail();

    array_shift($active_trail);
    if (count($active_trail) < 2) {
        return;
    }

    $variables['heading_title'] = $active_trail[0]['title'];
}

function _recat_preprocess_page_tabs(&$variables) {
    if (!isset($variables['tabs']) || !isset($variables['tabs']['#primary'])) {
        return;
    }

    if (empty($variables['tabs']['#primary'])) {
        $variables['tabs'] = false;
    }
}

function _recat_preprocess_page_main_content(&$variables) {
    $system_main = element_children($variables['page']['content']['system_main']);
    if (empty($system_main)) {
        $variables['has_main_content'] = false;
        return;
    }

    if (isset($variables['node'])) {
        $variables['has_main_content'] = false;
        foreach (array('body', 'picture', 'webform') as $field) {
            if (!isset($variables['node']->$field)) {
                continue;
            }

            if ($variables['node']->$field) {
                $variables['has_main_content'] = true;
                break;
            }
        }

        return;
    }
}

function _recat_preprocess_page_block_reference(&$variables) {
    if (!isset($variables['node']) || !isset($variables['node']->field_block)) {
        return;
    }

    if (empty($variables['node']->field_block)) {
        return;
    }

    $nodes_view = $variables['page']['content']['system_main']['nodes'];
    if (!isset($nodes_view[$variables['node']->nid])) {
        return;
    }

    $node_view = $nodes_view[$variables['node']->nid];
    if (!isset($node_view['field_block'])) {
        return;
    }

    $variables['block_reference'] = $node_view['field_block'];
    unset($node_view['field_block']);

    $variables['page']['content']['system_main']['nodes'][$variables['node']->nid] = $node_view;
}

function _recat_preprocess_page_news(&$variables) {
    if ((!isset($variables['node']) || $variables['node']->type !== 'news')
        && (!isset($variables['term']) || !isset($variables['term']->is_news))) {
        return;
    }

    $blocks = array();
    foreach (array('recat_news_tags', 'recat_news_categories') as $block_name) {
        $block = block_load('recat_news', $block_name);
        if (!$block) {
            continue;
        }

        $blocks[] = $block;
    }

    if (!empty($blocks)) {
        $variables['page']['sidebar'] = array_merge(_block_get_renderable_array(
            _block_render_blocks($blocks)
        ), $variables['page']['sidebar']);
    }

    $variables['is_colored'] = true;
}

function _recat_preprocess_page_search(&$variables) {
    if (!$variables['page']['content']['system_main']
        || !isset($variables['page']['content']['system_main']['search_form'])) {

    }

    $variables['page']['content']['system_main']['search_form']['#access'] = false;
}

function _recat_search_form_alter(&$form) {
    if (!isset($form['search_block_form'])) {
        return;
    }

    if (!isset($form['search_block_form']['#attributes'])) {
        $form['search_block_form']['#attributes'] = array();
    }

    $form['search_block_form']['#attributes']['placeholder']
        = _hs_resource_get('search.field.placeholder', 'plain', null, false);
}