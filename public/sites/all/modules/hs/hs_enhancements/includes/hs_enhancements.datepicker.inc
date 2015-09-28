<?php

function _hs_enhancements_datepicker_transform_date(&$form)
{
    $date_fields = array();
    if (!isset($form['submitted'])) {
        return;
    }

    foreach ($form['submitted'] as $field_key => $submitted) {
        if (!is_array($submitted)) {
            continue;
        }

        if ($submitted['#type'] !== 'date') {
            continue;
        }

        if (!isset($submitted['#datepicker']) || !$submitted['#datepicker']) {
            continue;
        }

        $form['submitted'][$field_key] = _hs_enhancements_datepicker_get($submitted);
        $date_fields[] = $field_key;
    }

    if (sizeof($date_fields)) {
        $form['#attached']['library'][] = array(
            'hs_enhancements', 'hs_enhancements_datepicker'
        );

        $form['#date_fields'] = $date_fields;

        $form['#submit'] = array_merge(
            (array) '_hs_enhancements_form_submit', $form['#submit']
        );
    }
}

function _hs_enhancements_form_submit($form, &$form_state)
{
    if (!isset($form['#date_fields'])) {
        return false;
    }

    foreach ($form['#date_fields'] as $date_field) {
        if (!isset($form_state['values']['submitted'][$date_field])) {
            continue;
        }

        $date_value = DateTime::createFromFormat(
            variable_get('date_format_tiny', 'd/m/Y'), $form_state['values']['submitted'][$date_field]
        );

        $form_state['values']['submitted'][$date_field] = array(
            'day' => $date_value->format('j'),
            'month' => $date_value->format('n'),
            'year' => $date_value->format('Y'),
        );
    }
}

function _hs_enhancements_datepicker_get($field)
{
    $field['#type'] = 'textfield';
    $field['#webform_component']['type'] = 'textfield';

    unset($field['#theme']);
    unset($field['#process']);
    unset($field['#element_validate']);
    unset($field['#attached']);

    $start_date = false;
    $end_date = false;

    if (isset($field['#start_date'])) {
        $start_date = strtotime($field['#start_date']);
    }

    if (isset($field['#end_date'])) {
        $end_date = strtotime($field['#end_date']);
    }

    // Add data attributes on element
    $field['#attributes'] = array_merge(isset($field['#attributes']) ? $field['#attributes'] : array(), array(
            'data-datepicker' => implode('|', array(
                $start_date,
                $end_date,
                format_date(strtotime('today'), 'tiny'),
            )),
            'data-format' => variable_get('date_format_tiny', 'd/m/Y'),
            'class' => array('datepicker'),
        )
    );

    if (isset($field['#default_value']) && !empty($field['#default_value'])) {
        if (is_array($field['#default_value'])) {
            $time = strtotime(sprintf('%s/%s/%s',
                $field['#default_value']['year'], $field['#default_value']['month'], $field['#default_value']['day']));
        } else {
            $time = strtotime($field['#default_value']);
        }

        $field['#default_value'] = format_date($time, 'tiny');
    }

    return $field;
}

function _hs_enhancements_month_names_untranslated()
{
    static $month_names;
    if (empty($month_names)) {
        $month_names = array(
            'January',
            'February',
            'March',
            'April',
            'May',
            'June',
            'July',
            'August',
            'September',
            'October',
            'November',
            'December',
        );
    }

    return $month_names;
}

function _hs_enhancements_month_names()
{
    $month_names = array();
    foreach (_hs_enhancements_month_names_untranslated() as $key => $month) {
        $month_names[$key] = t($month, array(), array('context' => 'month_name'));
    }

    return $month_names;
}

function _hs_enhancements_month_names_abbr($length = 3)
{
    $month_names = array();
    $context = sprintf('month_name_abbr%d', $length > 3 ? 3 : $length);

    foreach (_hs_enhancements_month_names_untranslated() as $key => $month) {
        $month_names[$key] = t(substr($month, 0, $length), array(), array('context' => $context));
    }

    return $month_names;
}

function _hs_enhancements_week_days_untranslated()
{
    static $weekdays;
    if (empty($weekdays)) {
        $weekdays = array(
            'Sunday',
            'Monday',
            'Tuesday',
            'Wednesday',
            'Thursday',
            'Friday',
            'Saturday',
        );
    }

    return $weekdays;
}

function _hs_enhancements_week_days()
{
    $weekdays = array();
    foreach (_hs_enhancements_week_days_untranslated() as $key => $day) {
        $weekdays[$key] = t($day, array(), array('context' => 'day_name'));
    }

    return $weekdays;
}

function _hs_enhancements_week_days_abbr($length = 3)
{
    $weekdays = array();
    $context = sprintf('day_name_abbr%d', $length > 3 ? 3 : $length);

    foreach (_hs_enhancements_week_days_untranslated() as $key => $day) {
        $weekdays[$key] = t(substr($day, 0, $length), array(), array('context' => $context));
    }

    return $weekdays;
}