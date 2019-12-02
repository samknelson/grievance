<?php

function sirius_page_taxonomy_access($vocabulary_machine_name) {
	if (user_access('sirius admin')) { return TRUE; }
	if (user_access('sirius domain admin')) { return TRUE; }
	return FALSE;
}

function sirius_page_taxonomy($vocabulary_machine_name) {
	return sirius_page_taxonomy_list($vocabulary_machine_name);
}

function sirius_page_taxonomy_list($vocabulary_machine_name) {
	$vocabulary = taxonomy_vocabulary_machine_name_load($vocabulary_machine_name);
	if (!$vocabulary) { return 'Vocabulary not found.'; }
	drupal_set_title($vocabulary->name);

	if (sirius_domain_taxonomy_is_multidomain($vocabulary_machine_name)) { $domain_nid = sirius_domain_nid(); }

	$view = variable_get("sirius_taxonomy_view_$vocabulary_machine_name", '');
	$display = variable_get("sirius_taxonomy_view_display_$vocabulary_machine_name", '');
	if (!$view || !$display) {
		return "Please set the system variables sirius_taxonomy_view_$vocabulary_machine_name and sirius_taxonomy_view_display_$vocabulary_machine_name";
	}

	return views_embed_view($view, $display, $vocabulary_machine_name, $domain_nid);
}

