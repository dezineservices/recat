<?php

function recat_library_ajax() {
    $output = array(
        'success' => true,
        'message' => array('code' => 200),
    );

    $settings = _recat_library_get_settings();
    
    try {
        list($current_page, $total_pages, $total, $nids)
            = _recat_library_get_nodes(array(), $settings['tags_query'], $settings['items_per_page']);

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

            $output['nodes'][] = array(
                'nid' => $nid,
                'title' => $node->title,
                'date' => format_date($node->created, $settings['date_format']),
                'url' => $field_file['hash'],
                'private' => $node_wrapper->field_private->value(),
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