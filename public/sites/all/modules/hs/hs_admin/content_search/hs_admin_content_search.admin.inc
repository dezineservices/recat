<?php

function hs_admin_content_search_form($form, $form_state)
{
    _hs_admin_content_search_init_node_module();

    if (isset($form_state['values']['operation']) && $form_state['values']['operation'] == 'delete') {
        return hs_admin_content_search_node_multiple_delete_confirm($form, $form_state, array_filter($form_state['values']['nodes']));
    }

    $form['filter'] = node_filter_form();
    $form['#submit'][] = 'node_filter_form_submit';
    $form['admin'] = hs_admin_content_search_admin_nodes();

    _hs_admin_content_search_filter_form($form, $form_state);

    drupal_set_title(hs_admin_content_search_form_title(), PASS_THROUGH);
    return $form;
}

function hs_admin_content_search_form_title()
{
    $query = _hs_admin_content_search_query_get();

    $format = 'Search results';
    if ($query) {
        $format .= ' for %title%';
    }

    return t($format, array(
        '%title%' => $query,
    ));
}

/**
 * Based on the node admin system from the Drupal Core.
 *
 * @see node_admin_nodes
 */
function hs_admin_content_search_admin_nodes()
{
    $admin_access = user_access('administer nodes');
    $destination = drupal_get_destination();

    // Build the 'Update options' form.
    $form['options'] = array(
        '#type' => 'fieldset',
        '#title' => t('Update options'),
        '#attributes' => array('class' => array('container-inline')),
        '#access' => $admin_access,
    );

    $options = array();
    foreach (module_invoke_all('node_operations') as $operation => $array) {
        $options[$operation] = $array['label'];
    }

    $form['options']['operation'] = array(
        '#type' => 'select',
        '#title' => t('Operation'),
        '#title_display' => 'invisible',
        '#options' => $options,
        '#default_value' => 'approve',
    );

    $form['options']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Update'),
        '#validate' => array('node_admin_nodes_validate'),
        '#submit' => array('node_admin_nodes_submit'),
    );


    // Enable language column if translation module is enabled or if we have any node with language.
    $multilanguage = (module_exists('translation') || db_query_range("SELECT 1 FROM {node} WHERE language <> :language", 0, 1, array(':language' => LANGUAGE_NONE))->fetchField());
    $languages = language_list();

    // Build the sortable header.
    $header = array(
        'title' => array('data' => t('Title'), 'field' => 'n.title'),
        'type' => array('data' => t('Type'), 'field' => 'n.type'),
        'author' => t('Author'),
        'status' => array('data' => t('Status'), 'field' => 'n.status'),
        'changed' => array('data' => t('Updated'), 'field' => 'n.changed', 'sort' => 'desc')
    );

    if ($multilanguage) {
        $header['language'] = array('data' => t('Language'), 'field' => 'n.language');
    }

    $header['operations'] = array('data' => t('Operations'));


    $query = db_select('node', 'n')
        ->extend('PagerDefault')
        ->extend('TableSort');

    hs_admin_content_search_build_filter_query($query);
    node_build_filter_query($query);

    $nids = $query
        ->fields('n', array('nid'))
        ->limit(50)
        ->orderByHeader($header)
        ->addTag('node_access')
        ->execute()
        ->fetchCol();

    $nodes = node_load_multiple($nids);


    if (!user_access('bypass node access')) {
        // If the user is able to view their own unpublished nodes, allow them
        // to see these in addition to published nodes. Check that they actually
        // have some unpublished nodes to view before adding the condition.
        if (user_access('view own unpublished content') && $own_unpublished = db_query('SELECT nid FROM {node} WHERE uid = :uid AND status = :status', array(':uid' => $GLOBALS['user']->uid, ':status' => 0))->fetchCol()) {
            $query->condition(db_or()
                    ->condition('n.status', 1)
                    ->condition('n.nid', $own_unpublished, 'IN')
            );
        }
        else {
            // If not, restrict the query to published nodes.
            $query->condition('n.status', 1);
        }
    }

    // Prepare the list of nodes.
    $options = array();
    foreach ($nodes as $node) {
        $lang_code = entity_language('node', $node);
        $lang_options = $lang_code != LANGUAGE_NONE && isset($languages[$lang_code])
            ? array('language' => $languages[$lang_code])
            : array();

        $options[$node->nid] = array(
            'title' => array(
                'data' => array(
                    '#type' => 'link',
                    '#title' => $node->title,
                    '#href' => 'node/' . $node->nid,
                    '#options' => $lang_options,
                    '#suffix' => ' ' . theme('mark', array('type' => node_mark($node->nid, $node->changed))),
                ),
            ),
            'type' => check_plain(node_type_get_name($node)),
            'author' => theme('username', array('account' => $node)),
            'status' => $node->status ? t('published') : t('not published'),
            'changed' => format_date($node->changed, 'short'),
        );
        if ($multilanguage) {
            if ($lang_code == LANGUAGE_NONE || isset($languages[$lang_code])) {
                $options[$node->nid]['language'] = $lang_code == LANGUAGE_NONE ? t('Language neutral') : t($languages[$lang_code]->name);
            }
            else {
                $options[$node->nid]['language'] = t('Undefined language (@langcode)', array('@langcode' => $lang_code));
            }
        }

        // Build a list of all the accessible operations for the current node.
        $operations = array();
        if (node_access('update', $node)) {
            $operations['edit'] = array(
                'title' => t('edit'),
                'href' => 'node/' . $node->nid . '/edit',
                'query' => $destination,
            );
        }
        if (node_access('delete', $node)) {
            $operations['delete'] = array(
                'title' => t('delete'),
                'href' => 'node/' . $node->nid . '/delete',
                'query' => $destination,
            );
        }
        $options[$node->nid]['operations'] = array();
        if (count($operations) > 1) {
            // Render an unordered list of operations links.
            $options[$node->nid]['operations'] = array(
                'data' => array(
                    '#theme' => 'links__node_operations',
                    '#links' => $operations,
                    '#attributes' => array('class' => array('links', 'inline')),
                ),
            );
        }
        elseif (!empty($operations)) {
            // Render the first and only operation as a link.
            $link = reset($operations);
            $options[$node->nid]['operations'] = array(
                'data' => array(
                    '#type' => 'link',
                    '#title' => $link['title'],
                    '#href' => $link['href'],
                    '#options' => array('query' => $link['query']),
                ),
            );
        }

    }

    if ($admin_access) {
        $form['nodes'] = array(
            '#type' => 'tableselect',
            '#header' => $header,
            '#options' => $options,
            '#empty' => t('No content available.'),
        );
    } else {
        $form['nodes'] = array(
            '#theme' => 'table',
            '#header' => $header,
            '#rows' => $options,
            '#empty' => t('No content available.'),
        );
    }


    $form['pager'] = array(
        '#theme' => 'pager',
    );


    return $form;
}

function _hs_admin_content_search_filter_form(&$form, &$form_state)
{
    $filter_weight = isset($form['filter']['#weight']) ? $form['filter']['#weight'] : 0;
    $form['filter']['#weight'] = $filter_weight - 1;

    $form['search'] = array(
      '#type' => 'fieldset',
      '#title' => t('Filter items by title'),
      '#weight' => $filter_weight,
    );

    $form['search']['query'] = array(
      '#type' => 'textfield',
      '#default_value' => isset($form_state['values']['query'])
        ? $form_state['values']['query']
        : _hs_admin_content_search_query_get(),
    );

    $form['search']['actions'] = array(
        '#type' => 'actions',
    );

    $form['#submit'][] = '_hs_admin_content_search_filter_form_submit';
    $form['search']['actions']['search'] = array(
      '#type' => 'submit',
      '#value' => t('Search'),
      '#submit' => array(
          '_hs_admin_content_search_filter_form_submit',
      ),
    );

    $form['search']['actions']['clear'] = array(
      '#type' => 'submit',
      '#value' => t('Clear'),
      '#submit' => array(
          '_hs_admin_content_search_query_reset',
      ),
    );
}

function _hs_admin_content_search_filter_form_submit($form, &$form_state)
{
    $query = isset($form_state['values']['query'])
        ? $form_state['values']['query']
        : NULL;

    if (!_hs_admin_content_search_query_is_valid($query)) {
        drupal_set_message(t('Invalid search term'), 'error');

        return;
    }

    $form_state['redirect'] = array(
        'admin/content/search',
        array(
            'query' => array(
                's' => $query,
            ),
        ),
    );
}

/**
 * @see node_multiple_delete_confirm()
 */
function hs_admin_content_search_node_multiple_delete_confirm($form, $form_state, $nodes)
{
    $confirmation = node_multiple_delete_confirm($form, $form_state, $nodes);

    // Workaround for fatal error (function not found)
    $callback_replacements = array(
        'node_multiple_delete_confirm' => 'hs_admin_content_search_node_multiple_delete_confirm',
    );

    $confirmation['#submit'] = str_replace(
        array_keys($callback_replacements), $callback_replacements,
        $confirmation['#submit']
    );

    return $confirmation;
}

/**
 * @see node_multiple_delete_confirm_submit()
 */
function hs_admin_content_search_node_multiple_delete_confirm_submit($form, $form_state)
{
    _hs_admin_content_search_init_node_module();

    node_multiple_delete_confirm_submit($form, $form_state);
}
