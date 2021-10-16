<?php

function sirius_bulk_node_tab_recipients_contacts_access($bulk_node) {
	if (sirius_domain_nid() && !sirius_domain_component_enabled('bulk')) { return FALSE; }
	return TRUE;
}

function sirius_bulk_node_tab_recipients_contacts($bulk_node) {
	return drupal_get_form('sirius_bulk_node_tab_recipients_contacts_form', $bulk_node);
}

function sirius_bulk_node_tab_recipients_contacts_form($form, &$form_state, $bulk_node) {
	$form_state['bulk_nid'] = $bulk_node->nid;

	$contact_definition_json = sirius_contact_definition();
	$contact_definition_form = sirius_jsonfield_util_form_get($contact_definition_json, 'default');

	sirius_mightysearch_form_prepare(
		$form, 
		$form_state, 
		$event_node->nid,
		array(
			'sirius_contact' => array('default' => $contact_definition_form),
			'sirius_worker' => array('default' => array()),
		),
		array(
			'bulk_recipient_add' => array(
				'title' => 'Add Bulk Recipient',
				'bulk_nid' => $bulk_node->nid,
			),
			'bulk_recipient_remove' => array(
				'title' => 'Remove Bulk Recipient',
				'bulk_nid' => $bulk_node->nid,
			),
			'export' => array('title' => 'Export'),
			'tag_add' => array('title' => 'Add a Tag'),
			'tag_remove' => array('title' => 'Remove a Tag'),
		)
	);

	return $form;
}

function sirius_bulk_node_tab_recipients_contacts_form_submit($form, &$form_state) {
	sirius_mightysearch_form_submit($form, $form_state);
}
