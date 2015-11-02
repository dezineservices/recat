<?php

function recat_activity_ajax() {
    $output = array(
        'success' => true,
        'message' => array('code' => 200),
    );

    $settings = _recat_activity_get_settings();
    
    try {
        $tid = filter_input(INPUT_GET, 'tid', FILTER_SANITIZE_URL);
        if (!$tid) {
            throw new Exception('recat_activity.bad_request', 400);
        }

        $term = taxonomy_term_load($tid);
        if (!$term) {
            throw new Exception('recat_activity.no_results_message', 404);
        }

        list($current_page, $total_pages, $total, $nids)
            = _recat_activity_get_nodes($tid, $settings['items_per_page']);

        if (empty($nids)) {
            throw new Exception('recat_activity.no_results_message', 404);
        }

        $nodes = node_load_multiple(array_keys($nids));
        if (empty($nodes)) {
            throw new Exception('recat_activity.no_results_message', 404);
        }

        $output['total'] = $total;
        $output['pager'] = array('current' => $current_page, 'pages' => $total_pages);
        $output['nodes'] = array();

        $term_wrapper = entity_metadata_wrapper('taxonomy_term', $term);
        $field_continent = $term_wrapper->field_continent->value();

        foreach ($nodes as $nid => $node) {
            $node_wrapper = entity_metadata_wrapper('node', $node);

            $field_status = $node_wrapper->field_status->label();
            $field_activity = $node_wrapper->field_activity->value();

            if (module_exists('i18n_taxonomy')) {
                $field_activity = i18n_taxonomy_localize_terms(array($field_activity));
                $field_activity = array_shift($field_activity);
            }

            $output['nodes'][] = array(
                'nid' => $nid,
                'title' => $node->title,
                'status' => $field_status,
                'statusClass' => strtolower(str_replace(' ', '', $field_status)),
                'country' => $term->name,
                'continent' => $field_continent,
                'url' => url(sprintf('node/%d', $nid)),
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