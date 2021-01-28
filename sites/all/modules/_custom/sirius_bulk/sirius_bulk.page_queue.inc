<?php

function sirius_bulk_node_tab_queue($bulk_node) {
	return drupal_get_form('sirius_bulk_node_tab_queue_form', $bulk_node);
}

function sirius_bulk_node_tab_queue_form($form, &$form_state, $bulk_node) {
	drupal_add_css(drupal_get_path('module', 'sirius_bulk') . '/css/bulk.css');

	$form['summary'] = array('#markup' => sirius_bulk_queue_summary_render($bulk_node));

	$contact_definition_json = sirius_contact_definition();
	$contact_definition_form = sirius_jsonfield_util_form_get($contact_definition_json, 'default');

	sirius_mightysearch_form_prepare(
		$form, 
		$form_state, 
		$event_node->nid,
		'Recipient Search', 
		array(
			'sirius_contact' => array(
				'default' => $contact_definition_form
			),
			'sirius_bulk' => array(
				'bulk_nid' => $bulk_node->nid,
			),
		),
		array(
			'export' => array('title' => 'Export'),
			'tag_add' => array('title' => 'Add a Tag'),
			'tag_remove' => array('title' => 'Remove a Tag'),
		)
	);

	return $form;
}

function sirius_bulk_node_tab_queue_form_submit($form, &$form_state) {
	sirius_mightysearch_form_submit($form, $form_state);
}
