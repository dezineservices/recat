<?php

function recat_core_taxonomy_page($term) {
    if (_recat_news_is_taxonomy_page($term)) {
        module_load_include('inc', 'recat_news', 'recat_news.pages');
        return recat_news_taxonomy_page($term);
    }

    if (_recat_library_is_taxonomy_page($term)) {
        module_load_include('inc', 'recat_library', 'recat_library.pages');
        return recat_library_taxonomy_page($term);
    }

    if (module_exists('i18n_taxonomy')) {
        module_load_include('inc', 'i18n_taxonomy', 'i18n_taxonomy.pages');
        return i18n_taxonomy_term_page($term);
    }

    return taxonomy_term_page($term);
}