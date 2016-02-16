<?php

function recat_library_ajax() {
    $output = array(
        'success' => true,
        'message' => array('code' => 200),
    );

    $settings = _recat_library_get_settings();
    
    try {
        $condition = filter_input(INPUT_GET, 'condition', FILTER_SANITIZE_URL);
        $tids = filter_input(INPUT_GET, 'tid', FILTER_SANITIZE_URL);

        if (!$condition) {
            $condition = $settings['tags_query'];
        }

        switch (strtoupper($condition)) {
            case 'OR':
                $condition = 'OR';
                break;
            default:
                $condition = 'AND';
        }

        list($current_page, $total_pages, $total, $nids)
            = _recat_library_get_nodes($tids ? explode(',', $tids) : array(), $condition, $settings['items_per_page']);

        if (empty($nids)) {
            throw new Exception('recat_library.no_results_message', 404);
        }

        $nodes = node_load_multiple(array_keys($nids));
        if (empty($nodes)) {
            throw new Exception('recat_library.no_results_message', 404);
        }

        $output['total'] = $total;
        $output['pager'] = array('current' => $current_page, 'pages' => $total_pages);
        $output['nodes'] = array();

        foreach ($nodes as $nid => $node) {
            $node_wrapper = entity_metadata_wrapper('node', $node);

            $field_file = $node_wrapper->field_file->value();
            $field_tags = $node_wrapper->field_tags->value();

            if (module_exists('i18n_taxonomy')) {
                $field_tags = i18n_taxonomy_localize_terms($field_tags);
            }

            $tags = array();
            foreach ($field_tags as $tag) {
                if (!$tag) {
                    continue;
                }

                $tags[] = $tag->name;
            }

            if ($node_wrapper->field_private->value() === false) {
                $url = url(_recat_library_get_url('download', $field_file['hash']), array(
                    'absolute' => true,
                ));
            } else {
                $url = url(_recat_wf_overlay_get_url($settings['webform']), array(
                    'absolute' => true,
                    'query' => array('nid' => $nid),
                ));
            }

            $output['nodes'][] = array(
                'nid' => $nid,
                'title' => $node->title,
                'tags' => !empty($tags) ? join(', ', $tags) : null,
                'date' => format_date($node->created, $settings['date_format']),
                'private' => $node_wrapper->field_private->value(),
                'url' => $url,
            );
        }

    } catch (Exception $e) {
        $output['message'] = array(
            'code' => $e->getCode(),
            'text' => _hs_resource_get($e->getMessage(), 'plain', null, false),
        );
        
        $output['success'] = false;
    }
    
    return $output;
}