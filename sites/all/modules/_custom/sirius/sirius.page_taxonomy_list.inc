<?php

function sirius_page_taxonomy_access($vocabulary_machine_name) {
	// Component must be enabled
	$info = sirius_taxonomy_settings($vocabulary_machine_name);
	if (!$info) { return FALSE; }
	if ($info['component']) {
		if (!sirius_domain_component_enabled($info['component'])) { return FALSE; }
	}

	if (user_access('sirius admin')) { return TRUE; }
	if (user_access('sirius domain admin')) { return TRUE; }

	return FALSE;
}

function sirius_page_taxonomy($vocabulary_machine_name) {
	return sirius_page_taxonomy_list($vocabulary_machine_name);
}

function sirius_page_taxonomy_list($vocabulary_machine_name) {
	sirius_backlink_add('sirius/taxonomy', array('name' => 'All Drop-Down Lists'));

	$vocabulary = taxonomy_vocabulary_machine_name_load($vocabulary_machine_name);
	if (!$vocabulary) { return 'Vocabulary not found.'; }
	drupal_set_title($vocabulary->name);

	if (sirius_domain_taxonomy_is_multidomain($vocabulary_machine_name)) { $domain_nid = sirius_domain_nid(); }

  $settings = sirius_taxonomy_settings($vocabulary_machine_name);

	return views_embed_view($settings['view'], $settings['display'], $vocabulary_machine_name, $domain_nid);
}

