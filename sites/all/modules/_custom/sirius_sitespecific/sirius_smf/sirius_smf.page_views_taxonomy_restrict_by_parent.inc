<?php

function sirius_smf_page_views_taxonomy_restrict_by_parent_config() {
	return drupal_get_form('sirius_smf_page_views_taxonomy_restrict_by_parent_config_form');
}

function sirius_smf_page_views_taxonomy_restrict_by_parent_config_form($form, &$form_state) {
	$form['sirius_views_taxonomy_restrict_by_parent'] = array(
		'#type' => 'textarea',
		'#size' => 20,
		'#title' => 'Settings',
		'#description' => 'Each row view_machine_name|field_name|tid, i.e. test_smf_worker_month_search_show_certain_tags|field_sirius_contact_tags|1558',
		'#default_value' => variable_get('sirius_views_taxonomy_restrict_by_parent', ''),
	);

	$form['submit'] = array(
		'#type' => 'submit', 
		'#value' => t('Submit')
	);

	return $form;
}

function sirius_smf_page_views_taxonomy_restrict_by_parent_config_form_submit($form, &$form_state) {
	variable_set('sirius_views_taxonomy_restrict_by_parent', $form_state['values']['sirius_views_taxonomy_restrict_by_parent']);
}