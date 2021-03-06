<?php

function sirius_event_page_participant_mightysearch($event_node, $rule, $lite = FALSE) {
	$prole_tid = $_REQUEST["prole"];
	if (!$prole_tid) {
		$html .= "To register new participants for this event, please select a role from the list above.";
		return $html;
	} 
	return drupal_get_form('sirius_event_page_participant_mightysearch_form', $event_node, $prole_tid, $lite);
}

function sirius_event_page_participant_mightysearch_form($form, &$form_state, $event_node, $prole_tid, $lite) {
	$form_state['event_nid'] = $event_node->nid;
	$form_state['prole_tid'] = $prole_tid;
	$form_state['lite'] = $lite;
	$participant_role_term = taxonomy_term_load($prole_tid);
	$statuses = sirius_event_participant_role_status_tids($prole_tid);
	$domain_nid = sirius_domain_nid();

	$contact_definition_json = sirius_contact_definition();
	$contact_definition_form = sirius_jsonfield_util_form_get($contact_definition_json, 'default');

	$participant_definition_node = sirius_event_survey_get($event_node);
	if ($participant_definition_node) { 
		$participant_definition_json = sirius_jsonfield_util_node_json_get($participant_definition_node);
		$participant_definition_form = sirius_jsonfield_util_form_get($participant_definition_json, 'default');
	}

	$actions = array();
	if (!$lite) {
		$actions['tag_add'] = array('title' => 'Add a Tag');
		$actions['tag_remove'] = array('title' => 'Remove a Tag');
		$actions['export'] = array('title' => 'Export');
		$actions['report'] = array('title' => 'Report');
		$actions['change_pstatus'] = array(
			'title' => 'Change Participant Status',
			'event_nid' => $event_node->nid,
			'prole_tid' => $prole_tid,
			'pstatus_options' => sirius_event_participant_role_status_tids($prole_tid),
		);
		$actions['delete_participant'] = array(
			'title' => 'Delete Participant Record',
		);
	}

	sirius_mightysearch_form_prepare(
		$form, 
		$form_state, 
		$event_node->nid,
		'Participant Search', 
		array(
			'sirius_contact' => array('default' => $contact_definition_form),
			'sirius_event_participant' => array(
				'survey' => $participant_definition_form, 
				'event_nid' => $event_node->nid,
				'prole_tid' => $prole_tid,
			),
		),
		$actions
	);

	return $form;
}

function sirius_event_page_participant_mightysearch_form_submit($form, &$form_state) {
	sirius_mightysearch_form_submit($form, $form_state);
}
