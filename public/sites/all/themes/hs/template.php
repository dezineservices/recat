<?php

function hs_html_head_alter(&$head_elements) {
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8'
  );
}

function hs_css_alter(&$css) {
  unset($css[drupal_get_path('module', 'system') . '/system.theme.css']);
}

function hs_js_alter(&$javascript) {
  $jquery = drupal_get_path('theme', 'hs') . '/design/scripts/vendor/jquery.min.js';
  if (isset($javascript['misc/jquery.js'])) {
    $javascript[$jquery] = $javascript['misc/jquery.js'];
    $javascript[$jquery]['version'] = '1.11.1';
    $javascript[$jquery]['data'] = $jquery;

    unset($javascript['misc/jquery.js']);
  }
}

function hs_preprocess_html(&$variables) {
  global $theme;

  if (!isset($variables['rdf']) || !is_object($variables['rdf'])) {
    $variables['rdf'] = new StdClass();
  }

  if (module_exists('rdf')) {
    $variables['doctype'] = '<!DOCTYPE html PUBLIC "-//W3C//DTD HTML+RDFa 1.1//EN">' . "\n";
    $variables['rdf']->version = 'version="HTML+RDFa 1.1"';
    $variables['rdf']->namespaces = $variables['rdf_namespaces'];
    $variables['rdf']->profile = ' profile="' . $variables['grddl_profile'] . '"';
  }
  else {
    $variables['doctype'] = '<!DOCTYPE html>' . "\n";
    $variables['rdf']->version = '';
    $variables['rdf']->namespaces = '';
    $variables['rdf']->profile = '';
  }

  $variables['modernizr'] = theme('html_tag', _hs_get_script_element('vendor/modernizr.min.js'));
  $variables['html5shim'] =
    "\n<!--[if lt IE 9]>\n" .
    theme('html_tag', _hs_get_css_element('ie.css', $theme)) .
    theme('html_tag', _hs_get_script_element('vendor/html5shiv.min.js')) .
    "<![endif]-->\n";

  drupal_add_html_head(array(
      '#tag' => 'meta',
      '#attributes' => array(
        'name' => 'viewport',
        'content' => 'width=device-width, initial-scale=1, maximum-scale=1',
      )
    ), 'mobile'
  );

  if (theme_get_setting('hs_debug_grid')) {
    drupal_add_js(array('hsDebugGrid' => theme_get_setting('hs_grid_columns')), 'setting');
  }
}

function hs_preprocess_page(&$variables) {
  $variables['main_menu'] = menu_tree_output(_hs_menu_tree('main-menu'));
    $variables['footer_menu'] = menu_tree_output(_hs_menu_tree('menu-footer-menu'));
}

function hs_preprocess_block(&$variables) {
  $variables['block_html_id'] = _hs_block_id_cleanup($variables['block_html_id']);

  if ($variables['block_id'] === 1) {
    $variables['classes_array'][] = 'first';
  }

  if (isset($variables['block']->last_in_region)) {
    $variables['classes_array'][] = 'last';
  }

  $variables['classes_array'][] = $variables['block_zebra'];
  $variables['title_attributes_array']['class'][] = 'block-title';
}

function _hs_menu_tree($menu_name) {
  global $language;

  $menu_item = menu_get_item();
  $menu_tree = menu_tree_all_data($menu_name, null, 2);

  foreach ($menu_tree as $key => $item) {
    if ($item['link']['language'] != $language->language) {
      unset($menu_tree[$key]);
    }
  }

  _hs_menu_tree_active_trail($menu_tree, $menu_item);

  return $menu_tree;
}

function _hs_menu_tree_active_trail(&$tree_data, $menu_item) {
    $is_front_page = drupal_is_front_page();

  foreach ($tree_data as $key => &$item) {
    if (!empty($item['link']) && ($item['link']['href'] === $menu_item['href']
            || ($item['link']['href'] === '<front>' && $is_front_page))) {
      $item['link']['in_active_trail'] = true;
      //return true;
    }
    else {
      if (!empty($item['below'])) {
        if (_hs_menu_tree_active_trail($item['below'], $menu_item)) {
          $item['link']['in_active_trail'] = true;
          //return true;
        }
      }
    }
  }

  //return false;
}

function _hs_block_id_cleanup($id) {
  $parts = explode('-', str_replace(array(
    'block-',
    ''
  ), '', $id));

  if (!$parts) {
    return $id;
  }

  return array_shift($parts) . implode('', array_map(function ($item) {
    return ucfirst($item);
  }, $parts));
}

function _hs_get_css_element($src, $theme = 'hs') {
  global $base_url;

  return array(
    'element' => array(
      '#tag' => 'link',
      '#attributes' => array(
        'href' => sprintf(
          '%s/%s/design/css/%s', $base_url, drupal_get_path('theme', $theme), $src),
        'rel' => 'stylesheet',
        'type' => 'text/css',
      ),
    ),
  );
}

function _hs_get_script_element($src, $theme = 'hs') {
  global $base_url;

  return array(
    'element' => array(
      '#tag' => 'script',
      '#value' => '',
      '#attributes' => array(
        'src' => sprintf(
          '%s/%s/design/scripts/%s', $base_url, drupal_get_path('theme', $theme), $src),
      ),
    ),
  );
}