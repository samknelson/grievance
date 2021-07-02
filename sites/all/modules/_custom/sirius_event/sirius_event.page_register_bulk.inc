<?php

function sirius_node_tab_event_participants_register_bulk($event_node) {
	$prole_tid = $_REQUEST["participant_role_tid"];
	if (!$prole_tid) {
		$html .= "To register new participants for this event, please select a role from the list above.";
		return $html;
	} 
	return drupal_get_form('sirius_node_tab_event_participants_register_bulk_form', $event_node, $prole_tid);
}

function sirius_node_tab_event_participants_register_bulk_form($form, &$form_state, $event_node, $prole_tid) {
	$form_state['event_nid'] = $event_node->nid;
	$form_state['prole_tid'] = $prole_tid;
	$participant_role_term = taxonomy_term_load($prole_tid);
	$statuses = sirius_event_participant_role_status_tids($prole_tid);
	$domain_nid = sirius_domain_nid();

	$contact_definition_json = sirius_contact_definition();
	$contact_definition_form = sirius_jsonfield_util_form_get($contact_definition_json, 'default');

	sirius_mightysearch_form_prepare(
		$form, 
		$form_state, 
		$event_node->nid,
		array('sirius_contact' => 
			array(
				'default' => $contact_definition_form
			)
		),
		array(
			'search' => array(
				'title' => 'Search',
			),
			'register' => array(
				'title' => 'Register',
				'event_nid' => $event_node->nid,
				'prole_tid' => $prole_tid,
				'pstatus_options' => sirius_event_participant_role_status_tids($prole_tid),
			),
		)
	);

	return $form;
}

function sirius_node_tab_event_participants_register_bulk_form_submit($form, &$form_state) {
	sirius_mightysearch_form_submit($form, $form_state);
}
