<?php

function recat_library_download($hash) {
    $fid = _recat_library_get_file($hash);
    if (!$fid) {
        drupal_not_found();
        drupal_exit();
    }

    $file = file_load($fid->fid);
    if (!$file) {
        drupal_not_found();
        drupal_exit();
    }

    if (!file_exists($file->uri)) {
        drupal_not_found();
        drupal_exit();
    }

    $headers = file_download_headers($file->uri);
    if (empty($headers)) {
        drupal_not_found();
        drupal_exit();
    }

    $headers = array_merge($headers, array(
        'Content-Disposition' => 'attachment; filename="' . mime_header_encode($file->filename) . '"',
    ));

    file_transfer($file->uri, $headers);
    drupal_exit();
}