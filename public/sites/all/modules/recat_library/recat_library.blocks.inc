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

function _recat_library_block_view() {
    $vocabulary = _recat_library_get_vocabulary();
    $tags = _recat_library_get_tags();

    if (empty($tags)) {
        return;
    }

    $build = _recat_library_block_get_build();
    $build['#tags'] = array(
        '#theme' => 'recat_library_tags',
        '#title' => $vocabulary->name,
        '#tags' => $tags,
    );

    return $build;
}

function _recat_library_block_simple_view() {
    return _recat_library_block_get_build();
}

function _recat_library_block_get_build() {
    return array(
        '#attached' => array(
            'js' => array(
                array(
                    'data' => array('recatLibrary' => array(
                        'serviceUrl' => url(_recat_library_get_url(), array('absolute' => true)),
                    )),
                    'type' => 'setting',
                ), array(
                    'data' => drupal_get_path('module', 'recat_library') . '/js/recat_library.min.js',
                    'type' => 'file',
                )
            ),
            'library' => array(
                array('recat_core', 'recat_core_angularjs'),
            ),
        ),
        '#theme' => 'recat_library',
    );
}