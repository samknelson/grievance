<?php

function sirius_page_contacts() {
	return drupal_get_form('sirius_page_contacts_form');
}

function sirius_page_contacts_form($form, &$form_state) {
	$contact_definition_json = sirius_contact_definition();
	$contact_definition_form = sirius_jsonfield_util_form_get($contact_definition_json, 'default');

	sirius_mightysearch_form_prepare(
		$form, 
		$form_state, 
		'main_contact_search',
		'Contact Search', 
		array(
			'sirius_contact' => array('default' => $contact_definition_form),
			'sirius_worker' => array('default' => array()),
		),
		array(
			'export' => array('title' => 'Export'),
			'report' => array('title' => 'Report'),
			'tag_add' => array('title' => 'Add a Tag'),
			'tag_remove' => array('title' => 'Remove a Tag'),
			'geocode' => array('title' => 'Geocode'),
			'delete' => array('title' => 'Delete'),
		)
	);

	return $form;
}

function sirius_page_contacts_form_submit($form, &$form_state) {
	sirius_mightysearch_form_submit($form, $form_state);
}
