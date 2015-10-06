<?php

function recat_preprocess_html(&$variables) {
    _recat_preprocess_html_fonts();

}

function recat_preprocess_page(&$variables) {
    $variables['has_main_content'] = true;

    $variables['site_name_abbr'] = _hs_resource_get('global.site_name_abbr', 'plain', null, false);
    $variables['footer_copyright'] = str_replace(
        '{{YEAR}}', date('Y'), _hs_resource_get('global.footer_copyright', 'markup'));

    if(isset($variables['page']['content']['system_main']['default_message'])) {
        unset($variables['page']['content']['system_main']['default_message']);
    }

    _recat_preprocess_page_tabs($variables);
    _recat_preprocess_page_main_content($variables);
    _recat_preprocess_page_block_reference($variables);
}

function recat_preprocess_block(&$variables) {
    $variables['block_html_id'] = str_replace('BlocksRecatBlocks', '', $variables['block_html_id']);
    $variables['block_html_id'] = str_replace('RecatNews', '', $variables['block_html_id']);
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