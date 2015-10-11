<?php

function _recat_library_block_configure($key) {
    $form[$key] = array(
        '#type' => 'fieldset',
        '#title' => t('Library content settings'),
        '#tree' => true,
    );

    $form[$key]['settings_page'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="form-item">',
        '#suffix' => '</div>',
        '#markup' => t('Files library settings can be managed on the !url page.', array(
            '!url' => l('Library settings', 'admin/config/recat/library'),
        )),
    );

    return $form;
}

function _recat_library_block_save($form_state, $key) {
    // nothing to save
}

function _recat_library_block_view($key) {
    $vocabulary = _recat_library_get_vocabulary();
    $tags = _recat_library_get_tags();

    if (empty($tags)) {
        return;
    }

    return array(
        '#attached' => array(
            'js' => array(
                array(
                    'data' => array('recatLibrary' => array(
                        'serviceUrl' => url(_recat_library_get_url(), array('absolute' => true)),
                    )),
                    'type' => 'setting',
                ),
            ),
            'library' => array(
                array('recat_core', 'recat_core_angularjs'),
            ),
        ),
        '#theme' => 'recat_library',
        '#tags' => array(
            '#theme' => 'recat_library_tags',
            '#title' => $vocabulary->name,
            '#tags' => $tags,
        ),
    );
}