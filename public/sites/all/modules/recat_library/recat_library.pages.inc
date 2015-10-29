<?php

function recat_library_taxonomy_page($term) {
    if (taxonomy_term_edit_access($term)) {
        drupal_goto(sprintf('taxonomy/term/%d/edit', $term->tid));
    } else {
        drupal_not_found();
    }

    drupal_exit();
}