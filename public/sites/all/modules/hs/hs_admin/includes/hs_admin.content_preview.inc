<?php

/**
 * Implements hook_menu().
 *
 * Add preview menu callbacks for node objects.
 *
 * @return array
 */
function hs_admin_content_preview_menu()
{
    $items['node/%node/preview'] = array(
        'page callback' => 'hs_admin_content_preview_page',
        'page arguments' => array(1),
        'access callback' => 'node_access',
        'access arguments' => array('view', 1),
        'file' => 'includes/hs_admin.content_preview.inc',
        'type' => MENU_CALLBACK,
    );

    $items['node/preview'] = array(
        'page callback' => 'hs_admin_content_preview_page',
        'page arguments' => array(NULL),
        'access callback' => 'user_access',
        'access arguments' => array('access content'),
        'file' => 'includes/hs_admin.content_preview.inc',
        'type' => MENU_CALLBACK,
    );

    return $items;
}

/**
 * Implements hook_form_alter().
 *
 * Overrides the <b>Preview</b> submit handler in all node edit forms.
 * This will open a popup with a node preview,
 *
 * @param array $form
 * @param array $form_state
 * @param string $form_id
 *
 * @see node_form()
 */
function hs_admin_content_preview_form_alter(&$form, &$form_state, $form_id)
{
    $form_id_suffix = '_node_form';
    if ($form_id_suffix === substr($form_id, -strlen($form_id_suffix))) {
        form_load_include($form_state, 'inc', 'hs_admin', 'includes/hs_admin.content_preview');
        $form['actions']['preview']['#submit'] = array('hs_admin_content_preview_form_submit');

        $node = $form_state['node'];
        $module_path = drupal_get_path('module', 'hs_admin');

        if ($form_state['submitted']
            && isset($form_state['values']['op'])
            && $form_state['values']['op'] == t('Preview')
        ) {
            global $base_url;

            $nid_prefix = $node->nid > 0
                ? '/' . $node->nid
                : '';

            $preview_path = $base_url . '/node' . $nid_prefix . '/preview';

            drupal_add_js($module_path . '/js/hs_admin.content_preview.js');
            drupal_add_js(array('hsAdmin' => array(
                'contentPreview' => array(
                    'path' => $preview_path,
                    'nid' => $node->nid,
                    'bringToTop' => TRUE,
                ),
            )), 'setting');
        }
    }
}

/**
 * Submit handler for {@link node_form()}.
 *
 * This callback manages the cache of node objects for displaying the preview page.
 *
 * @param array $form
 * @param array $form_state
 */
function hs_admin_content_preview_form_submit($form, &$form_state)
{
    $node = node_form_submit_build_node($form, $form_state);
    $form_state['rebuild'] = TRUE;

    $cid = _hs_admin_content_preview_generate_cid($node);

    // The node is stored in database. We no longer need a cached version.
    if (isset($form_state['values']['op']) && $form_state['values']['op'] == t('Submit')) {
        cache_clear_all($cid, 'cache');
        return;
    }

    // Cache the updated version for display on the preview page.
    cache_set($cid, $node, 'cache', CACHE_TEMPORARY);
}

/**
 * Builds the preview page for a node.
 *
 * When a cached version of the node is available, this one will be used
 * to generate the preview. Otherwise, <em>$node</em> will be used.
 *
 * If no node object is passed to this function, and a node draft
 * (a node with nid 0) is stored in cache, this draft node will be used.
 *
 * @global object $user The current user object.
 * @param object $node A node object.
 *
 * @return array|int An array as expected by drupal_render(),
 *   or MENU_NOT_FOUND when node does not exist.
 */
function hs_admin_content_preview_page($node = NULL)
{
    global $user;
    
    // Check if there is an updated version of the node in cache
    $cid = _hs_admin_content_preview_generate_cid($node);
    $cache = cache_get($cid, 'cache');

    if ($cache) {
        // Prepare cached node to be displayed
        $node = $cache->data;
        _field_invoke_multiple('load', 'node', array($node->nid => $node));
    }

    if (!$node) {
        // Node does not exist in database, and no cached draft is available
        return MENU_NOT_FOUND;
    }

    // Hide toolbar
    $module_path = drupal_get_path('module', 'hs_admin');
    drupal_add_css($module_path . '/css/hs_admin.content_preview.css');

    // Temporarily switch to anonymous user
    drupal_save_session(FALSE);
    $current_user = $user;
    $user = drupal_anonymous_user();

    // Render the node
    $render = node_view($node);

    // Restore the current user
    $user = $current_user;
    drupal_save_session(TRUE);

    return $render;
}

/**
 * Generate a cache id, based on a node id and the current user id.
 *
 * When no node object is passed, a draft node id (0) will be used.
 *
 * @global object $user The current user object.
 * @param object $node A node object.
 * @return string A cache id string of the format <em>hs_admin:content_preview:node:[nid]:[uid]</em>.
 */
function _hs_admin_content_preview_generate_cid($node = NULL)
{
    global $user;

    $uid = $user->uid;
    $nid = $node ? $node->nid : 0;

    $cid = sprintf('hs_admin:content_preview:node:%d:%d', $nid, $uid);
    return $cid;
}
