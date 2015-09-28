<?php

function hs_admin_menu_schema_alter(&$schema)
{
    $schema['menu_links']['fields']['link_anchor'] = array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => false,
    );

    $schema['menu_links']['fields']['link_css_class'] = array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => false,
    );

    $schema['menu_links']['fields']['target_blank'] = array(
        'type' => 'int',
        'size' => 'tiny',
        'not null' => true,
        'default' => 0,
    );
}


function hs_admin_menu_menu_alter(&$items)
{
    $items['admin/structure/menu/manage/%menu']['page arguments'] = array('hs_admin_menu_overview_form', 4);
    $items['admin/structure/menu/manage/%menu']['file'] = 'includes/hs_admin.menu.inc';
    $items['admin/structure/menu/manage/%menu']['file path'] = drupal_get_path('module', 'hs_admin');
}

function hs_admin_menu_menu_link_alter(&$item, $menu)
{
    if (isset($item['target_blank'])) {
        if ($item['target_blank'] === 1) {
            $item['options']['attributes']['target'] = '_blank';
        } else {
            unset($item['options']['attributes']['target']);
        }
    }

    if (isset($item['link_css_class'])) {
        $css_class = trim(filter_xss($item['link_css_class']));
        if (empty($css_class)) {
            if (isset($item['options']['attributes']['class'])) {
                unset($item['options']['attributes']['class']);
            }
        } else {
            $item['options']['attributes']['class'] = array($css_class);
        }
    }

    if (isset($item['link_anchor'])) {
        $anchor = trim(filter_xss($item['link_anchor']));

        if (!empty($anchor)) {
            $item['options']['attributes']['data-target'] = $anchor;
        }
    }
}

function hs_admin_menu_form_alter(&$form, &$form_state, $form_id)
{
    if ($form_id !== 'menu_edit_item') {
        return;
    }

    $item = $form['original_item']['#value'];
    $form['link_anchor'] = array(
        '#type' => 'textfield',
        '#title' => t('Link anchor'),
        '#default_value' => isset($item['link_anchor'])
            ? $item['link_anchor'] : '',
    );

    $form['link_css_class'] = array(
        '#type' => 'textfield',
        '#title' => t('Css class name'),
        '#description' => t('The class name to be used for this link in the menu.'),
        '#default_value' => isset($item['link_css_class'])
            ? $item['link_css_class'] : '',
    );

    $form['target_blank'] = array(
        '#type' => 'checkbox',
        '#title' => t('Open link in new window / tab'),
        '#default_value' => isset($item['target_blank'])
            ? $item['target_blank'] : '',
    );
}

function hs_admin_menu_menu_link_insert($link)
{
    hs_admin_menu_menu_link_update($link);
}

function hs_admin_menu_menu_link_update($link)
{
    $fields = array();
    if (isset($link['link_anchor'])) {
        $fields['link_anchor'] = $link['link_anchor'];
    }

    if (isset($link['link_css_class'])) {
        $fields['link_css_class'] = filter_xss($link['link_css_class'], array());
    }

    if (isset($link['target_blank'])) {
        $fields['target_blank'] = $link['target_blank'];
    }

    if (empty($fields)) {
        return null;
    }

    db_update('menu_links')
        ->fields($fields)
        ->condition('mlid', $link['mlid'])
        ->execute();
}

/**
 * @see menu_overview_form()
 */
function hs_admin_menu_overview_form($form, &$form_state, $menu)
{
    module_load_include('admin.inc', 'menu');

    global $menu_admin;

    $form['#tree'] = TRUE;
    $form['#attached']['css'] = array(drupal_get_path('module', 'menu') . '/menu.css');

    $sql = "SELECT m.load_functions, m.to_arg_functions, m.access_callback, m.access_arguments, m.page_callback, m.page_arguments, m.delivery_callback, m.title, m.title_callback, m.title_arguments, m.type, m.description, ml.*
        FROM {menu_links} ml LEFT JOIN {menu_router} m ON m.path = ml.router_path
        WHERE ml.menu_name = :menu
        ORDER BY p1 ASC, p2 ASC, p3 ASC, p4 ASC, p5 ASC, p6 ASC, p7 ASC, p8 ASC, p9 ASC";
    $result = db_query($sql, array(':menu' => $menu['menu_name']), array('fetch' => PDO::FETCH_ASSOC));
    $links = $result->fetchAll(PDO::FETCH_ASSOC);

    $links_per_language = array();
    foreach ($links as $mlid => $link) {
        $links_per_language[$link['language']][] = $link;
    }

    foreach ($links_per_language as $language => $localized_links) {
        $tree = menu_tree_data($localized_links);
        $node_links = array();
        menu_tree_collect_node_links($tree, $node_links);

        // We indicate that a menu administrator is running the menu access check.
        $menu_admin = TRUE;
        menu_tree_check_access($tree, $node_links);
        $menu_admin = FALSE;

        $form[$language] = _hs_admin_menu_overview_tree_form($tree);
        $form[$language]['#menu'] = $menu;

        if (!element_children($form[$language])) {
            $form[$language]['#empty_text'] = t(
                'There are no menu links yet. <a href="@link">Add link</a>.',
                array('@link' => url('admin/structure/menu/manage/' . $form[$language]['#menu']['menu_name'] . '/add'))
            );
        }
    }

    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save configuration'),
    );

    return $form;
}

function _hs_admin_menu_overview_tree_form($tree, &$form = array())
{
    foreach ($tree as $data) {
        $item = $data['link'];

        // Don't show callbacks; these have $item['hidden'] < 0.
        if ($item && $item['hidden'] >= 0) {
            $mlid = 'mlid:' . $item['mlid'];

            $form[$mlid]['#item'] = $item;
            $form[$mlid]['#attributes'] = $item['hidden'] ? array('class' => array('menu-disabled')) : array('class' => array('menu-enabled'));
            $form[$mlid]['title']['#markup'] = l($item['title'], $item['href'], $item['localized_options']);

            if ($item['hidden']) {
                $form[$mlid]['title']['#markup'] .= ' (' . t('disabled') . ')';
            }

            elseif ($item['link_path'] == 'user' && $item['module'] == 'system') {
                $form[$mlid]['title']['#markup'] .= ' (' . t('logged in users only') . ')';
            }

            $form[$mlid]['hidden'] = array(
                '#type' => 'checkbox',
                '#title' => t('Enable @title menu link', array('@title' => $item['title'])),
                '#title_display' => 'invisible',
                '#default_value' => !$item['hidden'],
            );
            $form[$mlid]['weight'] = array(
                '#type' => 'weight',
                '#delta' => 50,
                '#default_value' => $item['weight'],
                '#title_display' => 'invisible',
                '#title' => t('Weight for @title', array('@title' => $item['title'])),
            );
            $form[$mlid]['mlid'] = array(
                '#type' => 'hidden',
                '#value' => $item['mlid'],
            );
            $form[$mlid]['plid'] = array(
                '#type' => 'hidden',
                '#default_value' => $item['plid'],
            );

            // Build a list of operations.
            $operations = array();
            $operations['edit'] = array('#type' => 'link', '#title' => t('edit'), '#href' => 'admin/structure/menu/item/' . $item['mlid'] . '/edit');

            // Only items created by the menu module can be deleted.
            if ($item['module'] == 'menu' || $item['updated'] == 1) {
                $operations['delete'] = array('#type' => 'link', '#title' => t('delete'), '#href' => 'admin/structure/menu/item/' . $item['mlid'] . '/delete');
            }

            // Set the reset column.
            elseif ($item['module'] == 'system' && $item['customized']) {
                $operations['reset'] = array('#type' => 'link', '#title' => t('reset'), '#href' => 'admin/structure/menu/item/' . $item['mlid'] . '/reset');
            }
            $form[$mlid]['operations'] = $operations;
        }

        if ($data['below']) {
            _hs_admin_menu_overview_tree_form($data['below'], $form);
        }
    }

    return $form;
}

/**
 * @see menu_overview_form_submit()
 */
function hs_admin_menu_overview_form_submit($form, &$form_state)
{
    _hs_admin_menu_overview_flatten_form($form, $form_state);

    module_load_include('admin', 'menu');
    menu_overview_form_submit($form, $form_state);
}

function _hs_admin_menu_overview_flatten_form(&$form, &$form_state)
{
    // Flatten menu item languages in form

    foreach (element_children($form) as $language) {
        if (!_hs_admin_menu_validate_language($language)) {
            continue;
        }

        $language_items = array();
        foreach (element_children($form[$language]) as $mlid) {
            $language_items[$mlid] = $form[$language][$mlid];
        }

        unset($form[$language]);
        $form = array_merge($language_items, $form);
    }


    // Flatten menu item languages in $form_state inputs

    $flattened_input = array();

    foreach ($form_state['input'] as $language => $language_items) {
        if (!_hs_admin_menu_validate_language($language)) {
            continue;
        }

        // Move the language items to the same level as their $language parent
        $flattened_input = array_merge($flattened_input, $language_items);

        // Remove the parent entry
        unset($form_state['input'][$language]);
    }

    $form_state['input'] = array_merge($flattened_input, $form_state['input']);
}

function _hs_admin_menu_validate_language($language, $allow_language_none = TRUE)
{
    if ($language === LANGUAGE_NONE) {
        return $allow_language_none;
    }

    $available_languages = language_list();
    $valid = array_key_exists($language, $available_languages);

    return $valid;
}

function hs_admin_menu_theme()
{
    return array(
        'hs_admin_menu_overview_form' => array(
            'file' => 'includes/hs_admin.menu.inc',
            'render element' => 'form',
        ),
    );
}

function theme_hs_admin_menu_overview_form($variables)
{
    $form = $variables['form'];

    $header = array(
        t('Menu link'),
        array('data' => t('Enabled'), 'class' => array('checkbox')),
        t('Weight'),
        array('data' => t('Operations'), 'colspan' => '3'),
    );

    $output = '';
    $tables_build = array();

    $available_languages = language_list();

    foreach (element_children($form) as $language) {
        $is_valid_language = _hs_admin_menu_validate_language($language);
        if (!$is_valid_language) {
            continue;
        }

        // Sort the available 'menu blocks' by the weight of their language, keeping language neutral menus on top.
        $has_language = LANGUAGE_NONE !== $language;
        $language_weight = $has_language
            ? $available_languages[$language]->weight
            : -255;

        $rows = array();

        foreach (element_children($form[$language]) as $mlid) {
            if (isset($form[$language][$mlid]['hidden'])) {

                $element =& $form[$language][$mlid];
                // Build a list of operations.
                $operations = array();
                foreach (element_children($element['operations']) as $op) {
                    $operations[] = array(
                        'data' => drupal_render($element['operations'][$op]),
                        'class' => array('menu-operations')
                    );
                }

                while (count($operations) < 2) {
                    $operations[] = '';
                }

                // Add special classes to be used for tabledrag.js.
                $element['plid']['#attributes']['class'] = array('menu-plid');
                $element['mlid']['#attributes']['class'] = array('menu-mlid');
                $element['weight']['#attributes']['class'] = array('menu-weight');

                // Change the parent field to a hidden. This allows any value but hides the field.
                $element['plid']['#type'] = 'hidden';

                $row = array();
                $row[] = theme('indentation', array('size' => $element['#item']['depth'] - 1)) . drupal_render(
                        $element['title']
                    );
                $row[] = array(
                    'data' => drupal_render($element['hidden']),
                    'class' => array('checkbox', 'menu-enabled')
                );
                $row[] = drupal_render($element['weight']) . drupal_render($element['plid']) . drupal_render(
                        $element['mlid']
                    );
                $row = array_merge($row, $operations);

                $row = array_merge(array('data' => $row), $element['#attributes']);
                $row['class'][] = 'draggable';
                $rows[] = $row;
            }
        }

        $table_id = 'menu-overview-' . $language;

        drupal_add_tabledrag($table_id, 'match', 'parent', 'menu-plid', 'menu-plid', 'menu-mlid', TRUE, MENU_MAX_DEPTH - 1);
        drupal_add_tabledrag($table_id, 'order', 'sibling', 'menu-weight');

        $table_title = $has_language
            ? $available_languages[$language]->name
            : 'Language neutral';

        $tables_build[$language] = array(
            '#weight' => $language_weight,
            '#theme' => 'table',
            '#header' => $header,
            '#rows' => $rows,
            '#attributes' => array('id' => $table_id),

            '#prefix' => "<h2>{$table_title}</h2>",
        );
    }

    $output .= drupal_render($tables_build);
    $output .= drupal_render_children($form);

    return $output;
}
