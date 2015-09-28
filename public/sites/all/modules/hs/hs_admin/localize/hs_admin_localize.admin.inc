<?php

function hs_admin_localize_page($form, &$form_state, $type = NULL)
{
    $conditions = array();

    $sources = hs_admin_localize_sources($type);
    if (empty($sources)) {
        drupal_not_found();
        drupal_exit();
    }

    if ($sources == array('node')) {
        $conditions['type'] = substr($type, strlen('node_'));
    }

    return _hs_admin_localize_overview_page($form, $form_state, $sources, $conditions);
}

function _hs_admin_localize_overview_page($form, &$form_state, array $sources, array $conditions = array())
{
    $header = array(
        'title' => array(
            'data' => t('Title'),
            'field' => 'title',
        ),
        'body' => t('Content'),
        'type' => array(
            'data' => t('Type'),
            'field' => 'entity_type',
        ),
        'operations' => t('Operations'),
    );

    $form['filter'] = hs_admin_localize_filter_form();
    $form['#submit'][] = 'hs_admin_localize_filter_form_submit';

    $form['#attached']['css'][] = drupal_get_path('module', 'hs_admin_localize') . '/css/hs_admin_localize.css';

    $query = _hs_admin_localize_build_union_query($sources, $conditions);
    hs_admin_localize_build_filter_query($query);

    $query
        ->extend('TableSort')
        ->orderByHeader($header)
        ->orderBy('eid');

    $query = $query
        ->extend('PagerDefault')
        ->limit(25);

    $result = $query->execute();

    $options = array();
    while ($obj = $result->fetch()) {
        $options[] = _hs_admin_localize_row_from_object($obj);
    }

    $form['table'] = array(
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $options,
        '#empty' => t('No content available.'),
    );

    $form['pager']['#theme'] = 'pager';

    return $form;
}

function hs_admin_localize_filters()
{
    $filters['query'] = array(
        'title' => t('Search by title or on content.', array(), array('context' => 'hs_admin_localize')),
    );

    return $filters;
}

function hs_admin_localize_build_filter_query(SelectQueryInterface $query)
{
    $filter_data = isset($_SESSION['hs_admin_localize_filter'])
        ? $_SESSION['hs_admin_localize_filter']
        : array();

    $filters = array_intersect_key($filter_data, hs_admin_localize_filters());

    /*$conditions = array();
    foreach ($filters as $key => $value) {
        $operator = '=';

        switch ($key) {
            case 'title':
                $operator = 'LIKE';
                $value = '%' . $value . '%';
                break;
        }

        $conditions[$key] = array(
            'value' => $value,
            'operator' => $operator,
        );
    }

    _hs_admin_localize_query_conditions($query, $conditions);*/

    if (isset($filters['query'])) {
        $fuzzy_search = '%' . $filters['query'] . '%';
        
        $search_condition = db_or();
        $search_condition->condition('title', $fuzzy_search, 'LIKE');
        $search_condition->condition('body_value', $fuzzy_search, 'LIKE');

        $query->condition($search_condition);
    }

    return $query;
}

function hs_admin_localize_filter_form()
{
    $filter_data = isset($_SESSION['hs_admin_localize_filter'])
        ? $_SESSION['hs_admin_localize_filter']
        : array();

    $form['filters'] = array(
        '#title' => t('Filter'),
        '#type' => 'fieldset',
    );

    $form['filters']['query'] = array(
        // '#title' => t('Title'),
        '#type' => 'textfield',
    );

    foreach (hs_admin_localize_filters() as $key => $options) {
        if (isset($filter_data[$key])) {
            $form['filters'][$key]['#default_value'] = $filter_data[$key];
        }
    }

    $form['filters']['actions']['#type']  = 'actions';

    $form['filters']['actions']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Filter'),
    );

    $form['filters']['actions']['reset'] = array(
        '#type' => 'submit',
        '#value' => t('Reset'),
    );

    return $form;
}

function hs_admin_localize_filter_form_submit($form, &$form_state)
{
    $conditions = array();

    switch ($form_state['values']['op']) {
        case t('Filter'):
            foreach (hs_admin_localize_filters() as $key => $filter) {
                if (isset($form_state['values'][$key]) && $form_state['values'][$key]) {
                    $conditions[$key] = $value = $form_state['values'][$key];
                }
            }

        case t('Reset'):
            $_SESSION['hs_admin_localize_filter'] = $conditions;
    }
}

function _hs_admin_localize_build_query($entity_type, $alias = NULL, $join_translation = FALSE)
{
    $entity_type = strtolower($entity_type);

    switch ($entity_type) {
        case 'node':
            $node_alias = $alias ?: 'node';
            $query = db_select('node', $node_alias);

            $query->addField($node_alias, 'nid', 'eid');
            $query->addField($node_alias, 'title');
            $query->addField($node_alias, 'type');

            $body_alias = $query->join('field_data_body', NULL, "%alias.entity_id = {$node_alias}.nid");
            $query->addField($body_alias, 'body_value');
            $query->addField($body_alias, 'body_format');

            if ($join_translation) {
                $node_src_alias = $query->leftJoin('node', NULL, "%alias.nid = {$node_alias}.tnid");
                $body_src_alias = $query->leftJoin('field_data_body', NULL, "%alias.entity_id = {$node_src_alias}.nid");

                $query->addField($node_src_alias, 'nid', 'eid_src');
                $query->addField($node_src_alias, 'title', 'title_src');
                $query->addField($body_src_alias, 'body_value', 'body_value_src');
                $query->addField($body_src_alias, 'body_format', 'body_format_src');
            }

            break;

        case 'taxonomy_vocabulary':
            $vocabulary_alias = $alias ?: 'taxonomy_vocabulary';
            $query = db_select('taxonomy_vocabulary', $vocabulary_alias);

            $query->addField($vocabulary_alias, 'vid', 'eid');
            $query->addField($vocabulary_alias, 'name', 'title');
            $query->addField($vocabulary_alias, 'description', 'body_value');

            break;

        case 'taxonomy_term':
            $term_alias = $alias ?: 'taxonomy_term_data';
            $query = db_select('taxonomy_term_data', $term_alias);

            $query->addField($term_alias, 'tid', 'eid');
            $query->addField($term_alias, 'name', 'title');
            $query->addField($term_alias, 'description', 'body_value');

            break;

        case 'hs_resource':
            $resource_alias = $alias ?: 'hs_resource';
            $query = db_select('hs_resource', $resource_alias);
            $translation_alias = $query->join('hs_resource_translation', NULL, "%alias.rid = {$resource_alias}.rid");
            
            $query->addField($resource_alias, 'term', 'title');
            $query->addField($resource_alias, 'rid', 'eid');
            $query->addField($translation_alias, 'value', 'body_value');
            $query->addField($translation_alias, 'format', 'body_format');

            break;

        default:
            return NULL;
    }

    $type_placeholder = ':entity_type_' . $entity_type;
    $query->addExpression($type_placeholder, 'entity_type', array($type_placeholder => $entity_type));

    $fixed_field_order = array('eid', 'entity_type', 'type', 'title', 'body_value', 'body_format');
    $query = $query->extend('HSStaticFieldOrder')->setFieldOrder($fixed_field_order);

    return $query;
}

function _hs_admin_localize_build_union_query(array $entity_types, array $conditions = array(), $join_translation = FALSE) // @todo
{
    $queries = array();
    foreach ($entity_types as $entity_type) {
        if ($entity_query = _hs_admin_localize_build_query($entity_type, NULL, $join_translation)) {
            $queries[$entity_type] = $entity_query;
        }
     }

    if (empty($queries)) {
        return FALSE;
    }

    $subquery = array_shift($queries);
    foreach ($queries as $query) {
        $subquery->union($query);
    }

    $union_query = Database::getConnection()
        ->select($subquery)
        ->fields(NULL, array('eid', 'title', 'body_value', 'body_format', 'entity_type', 'type'));

    if ($join_translation) {
        $union_query->fields(NULL, array('title_src', 'body_value_src', 'body_format_src'));
    }

    _hs_admin_localize_query_conditions($union_query, $conditions);

    return $union_query;
}

function _hs_admin_localize_row_from_object($object)
{
    $filter_data = isset($_SESSION['hs_admin_localize_filter'])
        ? $_SESSION['hs_admin_localize_filter']
        : array();

    $query = isset($filter_data['query']) ? $filter_data['query'] : NULL;

    $type = $object->entity_type;
    if (isset($object->type)) {
        $type .= ' (' . $object->type . ')';
    }

    // Build operation links.
    $destination = drupal_get_destination();
    $operations = array();

    $operations['view'] = array(
        'title' => t('view'),
        'href' => hs_admin_localize_entity_path($object, 'view'),
        'query' => $destination,
    );

    $operations['edit'] = array(
        'title' => t('edit'),
        'href' => hs_admin_localize_entity_path($object, 'edit'),
        'query' => $destination,
    );

    $operations['translate'] = array(
        'title' => t('translate'),
        'href' => hs_admin_localize_entity_path($object, 'translate'),
        'query' => $destination,
    );

    $body = isset($object->body_format) && $object->body_format
        ? check_markup($object->body_value, $object->body_format)
        : $object->body_value;

    $title = hs_admin_localize_text_teaser($object->title, $query);
    $teaser = hs_admin_localize_text_teaser($body, $query);

    $row = array(
        'title' => $title,
        'body' => $teaser,
        'entity_type' => $type,
        'operations' => array(
            'data' => array(
                '#theme' => 'links__node_operations',
                '#links' => $operations,
                '#attributes' => array('class' => array('links', 'inline')),
            )
        ),
    );

    return $row;
}

function _hs_admin_localize_object_is_localizable($entity)
{
    // @todo move to query level to fix paging

    if ('node' == $entity->entity_type) {
        $entity->nid = $entity->eid;
        return _translation_tab_access($entity);
    }

    return TRUE;
}

function hs_admin_localize_entity_path($entity, $op = 'view')
{
    $identifier = $entity->eid;

    $args = array(
        '@entity_id' => $identifier,
        '@operation' => $op,
    );

    $fnc = 'hs_admin_localize_' . $entity->entity_type . '_path_structure';
    if (function_exists($fnc)) {
        $path_structure = $fnc($entity, $op, $args);
        $path = format_string($path_structure, $args);
        return $path;
    }

    return FALSE;
}

function hs_admin_localize_node_path_structure($entity, $op = 'view', array &$args = array())
{
    return 'node/@entity_id/@operation';
}

function hs_admin_localize_taxonomy_vocabulary_path_structure($entity, $op = 'view', array &$args = array())
{
    $vocabulary = taxonomy_vocabulary_load($entity->eid);
    if (!$vocabulary) {
        return FALSE;
    }

    $args['@machine_name'] = $vocabulary->machine_name;

    // Replace 'view' operation with the 'list' action
    if ($op == 'view') {
        $args['@operation'] = 'list';
    }

    return 'admin/structure/taxonomy/@machine_name/@operation';
}

function hs_admin_localize_taxonomy_term_path_structure($entity, $op = 'view', array &$args = array())
{
    return 'taxonomy/term/@entity_id/@operation';
}

function hs_admin_localize_hs_resource_path_structure($entity, $op = 'view', array &$args = array())
{
    return 'admin/config/regional/translate/resources/@entity_id';
}

/*function _hs_admin_localize_entity_page($form, $form_state, $type)
{
    $conditions = array();

    if (NULL !== entity_get_info($type)) {
        // Found entity of type $type
        $entity_type = $type;
    } elseif (FALSE !== node_type_load($type)) {
        // Found node of type $type
        $entity_type = 'node';
        $conditions['type'] = $type;
    } else {
        // No matching entity or node of type $type found
        return FALSE;
    }


    $header = array(
        'title' => t('Title'),
        'operations' => t('Actions'),
    );

    $destination = drupal_get_destination();

    $node_options = array();
    $nodes = entity_load($entity_type, FALSE, $conditions);
    foreach ($nodes as $node) {
        // Render an unordered list of operations links.
        $operations = array();
        $operations['translate'] = array(
            'title' => t('Translate'),
            'href' => hs_admin_localize_entity_path($node),
        );

        $node_options[$node->nid] = array(
            'title' => $node->title,
            'operations' => array(
                'data' => array(
                    '#theme' => 'links__node_operations',
                    '#links' => $operations,
                    '#attributes' => array('class' => array('links', 'inline')),
                ),
            ),
        );
    }


    $render = array();

    $render['table'] = array(
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $node_options,
        '#empty' => t('No content available.'),
    );

    return $render;
}

function _hs_admin_localize_taxonomy_page($form, $form_state)
{
    return array(); // @todo
}*/

function _hs_admin_localize_compare_title($a, $b)
{
    return strcmp($a->title, $b->title);
}
