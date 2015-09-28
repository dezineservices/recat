<?php

function recat_preprocess_html(&$variables) {
    _recat_preprocess_html_fonts();

}

function recat_preprocess_page(&$variables) {
    $variables['site_name_abbr'] = _hs_resource_get('global.site_name_abbr', 'plain', null, false);
    $variables['footer_copyright'] = str_replace(
        '{{YEAR}}', date('Y'), _hs_resource_get('global.footer_copyright', 'markup'));

    if(isset($variables['page']['content']['system_main']['default_message'])) {
        unset($variables['page']['content']['system_main']['default_message']);
    }
}

function recat_preprocess_block(&$variables) {
    $variables['block_html_id'] = str_replace('BlocksRecatBlocks', '', $variables['block_html_id']);
}

function recat_form_alter(&$form, &$form_state, $form_id) {
    switch ($form_id) {
        case 'search_block_form':
            _recat_search_form_alter($form);
            break;
    }
}

function _recat_preprocess_html_fonts() {
    drupal_add_css('//fonts.googleapis.com/css?family=Roboto+Condensed|Roboto:400,300,500,700', array(
        'type' => 'external',
        'weight' => -100,
    ));
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