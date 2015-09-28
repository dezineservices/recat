<?php

/**
 * Overview page for the resources management
 * 
 * @return type
 */
function hs_resource_admin_page()
{
    $rows = array();
    $translated = '<em>%s</em>';
    $untranslated = '<em class="locale-untranslated">%s</em>';

    $has_access = user_access('administer hs resource');
    
    $header = array(
        'term' => array('data' => t('Term'), 'field' => 'r.term'),
        'type' => array('data' => t('Format'), 'field' => 'r.type'),
        'file' => t('File'),
        'languages' => t('Languages'),
        'operations' => t('Operations'),
    );
    
    if (!$has_access) {
        unset($header['file']);
    }

    $resources = _hs_resource_admin_page_get_resources($header);
    $languages = array();

    foreach (language_list() as $language) {
        if (!$language->enabled) {
            continue;
        }

        $languages[$language->language] =
            sprintf($untranslated, $language->language);
    }

    foreach ($resources as $resource) {
        if (!isset($rows[$resource->rid])) {
            $rows[$resource->rid] = array(
                $resource->term,
                $resource->type,
                $resource->file,
                implode(' ', $languages),
                array(
                    'data' => array(
                        '#theme' => 'links__node_operations',
                        '#links' => array(
                            'translate' => array(
                                'title' => t('translate'),
                                'href' => sprintf('admin/config/regional/translate/resources/%d', $resource->rid),
                            ),
                        ),
                        '#attributes' => array('class' => array('links', 'inline')),
                    ),
                ),
            );
        }

        if (!$has_access) {
            unset($rows[$resource->rid][2]);
        }
        
        $rows[$resource->rid][3] = str_replace(
            sprintf($untranslated, $resource->language), sprintf($translated, $resource->language), $rows[$resource->rid][3]);
    }

    drupal_add_css(drupal_get_path('module', 'locale') . '/locale.css');

    return theme('table', array(
        'header' => $header,
        'rows' => $rows,
        'empty' => t('No resources.'),
    ));
}

/**
 * Administration page for a resource
 * 
 * @param type $r_translation
 * @return type
 */
function hs_resource_admin_translate_page($r_translation)
{
    drupal_set_title(
        t('Translate %term', array('%term' => $r_translation->term)), PASS_THROUGH);
    
    return drupal_get_form('hs_resource_admin_translate_form', $r_translation);
}

/**
 * Administration form a resource entry
 * 
 * @param type $form
 * @param array $form_state
 * @param type $r_translation
 * @return array
 */
function hs_resource_admin_translate_form($form, &$form_state, $r_translation)
{
    $form_state['r_translation'] = $r_translation;
    
    $form['#tree'] = true;
    $form['term'] = array(
        '#type' => 'item',
        '#title' => t('Term'),
        '#markup' => $r_translation->term,
    );
                
    foreach (language_list() as $language) {
        if (!$language->enabled) {
            continue;
        }
        
        $is_new = !isset($r_translation->{$language->language});
        
        $form[$language->language] = array();
        $form[$language->language]['value'] = array(
            '#type' => 'textarea',
            '#required' => true,
            '#title' => $language->name,
            '#rows' => 2,
            '#default_value' => !$is_new
                ? $r_translation->{$language->language}->value
                : ''
        );
                
        if ($r_translation->type === 'markup') {
            $form[$language->language]['value'] = array_merge(
                $form[$language->language]['value'], array(
                    '#type' => 'text_format',
                    '#format' => !$is_new && $r_translation->{$language->language}->format
                        ? $r_translation->{$language->language}->format
                        : 'filtered_html',
                ));
        }
    }
    
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
        '#weight' => 5,
        '#submit' => array('hs_resource_admin_translate_form_submit'),
    );
    
    return $form;
}

/**
 * Submit handler for a resource form
 * 
 * @param type $form
 * @param array $form_state
 */
function hs_resource_admin_translate_form_submit($form, &$form_state)
{
    $r_translation = $form_state['r_translation'];
    $values = $form_state['values'];
    $has_errors = false;
    
    foreach (language_list() as $language) {
        if (!$language->enabled) {
            continue;
        }
        
        if (!isset($values[$language->language])) {
            continue;
        }
        
        $is_markup = $r_translation->type === 'markup';
        if (!isset($r_translation->{$language->language})) {
            $r_translation->{$language->language} = (object) array(
                'language' => $language->language,
                'value' => $is_markup 
                    ? $values[$language->language]['value']['value']
                    : $values[$language->language]['value'],
                'format' => $is_markup ? 
                    $values[$language->language]['value']['format'] : '',
                'rtid' => null,
                'rid' => $r_translation->rid,
            );
        } else {
            if ($is_markup) {
                $r_translation->{$language->language}->value =
                    $values[$language->language]['value']['value'];
                
                $r_translation->{$language->language}->format =
                    $values[$language->language]['value']['format'];
            } else {
                $r_translation->{$language->language}->value =
                    $values[$language->language]['value'];
            }
        }
        
        if (!hs_resource_save_translation($r_translation->{$language->language}, $r_translation) && !$has_errors) {
            drupal_set_message(t('An error occured while saving resource, please try again.'), 'error');
            $has_errors = true;
        }
    }
    
    if (!$has_errors) {
        drupal_set_message(t('The resource %term has been saved.', array('%term' => $r_translation->term)));
    }
    
    $form_state['redirect'] = 'admin/config/regional/translate/resources';
}

function hs_resource_admin_cleanup_form($form, &$form_state)
{
    $question = t('Are you sure you want to cleanup untranslated resources?');
    return confirm_form(
        $form, $question, _hs_resource_get_url());
}

function hs_resource_admin_cleanup_form_submit($form, &$form_state)
{
    $query = db_select('hs_resource', 'r');
    $query->leftJoin('hs_resource_translation', 't', 't.rid = r.rid');
    
    $query->fields('r', array('rid'))
        ->condition('rtid', null, 'IS');
    
    $result = $query->execute()->fetchAllAssoc('rid');
    
    if (!$result) {
        drupal_set_message('Nothing has been deleted', 'warning');
        $form_state['redirect'] = _hs_resource_get_url();
        
        return;
    }
    
    $deleted = db_delete('hs_resource')
        ->condition('rid', array_keys($result))
        ->execute();
    
    drupal_set_message(format_plural($deleted, 
        '1 unused resource has been deleted.', 
        '@count unused resources have been deleted.'
    ));
    
    $form_state['redirect'] = _hs_resource_get_url();
}

/**
 * Return all available resources
 * 
 * @param type $header
 * @return type
 */
function _hs_resource_admin_page_get_resources($header)
{
    $query = db_select('hs_resource', 'r')
        ->extend('TableSort');

    $query->leftJoin('hs_resource_translation', 't', 'r.rid = t.rid');
    $query
        ->fields('r', array())
        ->fields('t', array('language'))
        ->orderByHeader($header);

    return $query->execute()->fetchAll();
}