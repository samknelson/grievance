<?php

function sirius_page_term_exim($tid) {
	return drupal_get_form('sirius_json_definition_exim_form', $tid, 'ALL', 'taxonomy_term');
}
