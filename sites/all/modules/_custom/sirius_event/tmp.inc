<?php

function sirius_node_tab_event_participants_register_byname($event_node) {
	$participant_role_tid = $_REQUEST["participant_role_tid"];
	if (!$participant_role_tid) {
		$html = sirius_node_tab_event_participants_quicknav($event_node, 'register/byname');
		$html .= "To register new participants for this event, please select a role from the list above.";
		return $html;
	} 
	return drupal_get_form('sirius_node_tab_event_participants_register_byname_form', $event_node, $participant_role_tid);
}

function sirius_node_tab_event_participants_register_byname_form($form, &$form_state, $event_node, $participant_role_tid) {
	$form_state['event_node'] = $event_node;
	$form_state['participant_role_tid'] = $participant_role_tid;
	$form['nav'] = array('#markup' => sirius_node_tab_event_participants_quicknav($event_node, 'register/byname'));

	$participant_role_term = taxonomy_term_load($participant_role_tid);
	$statuses = sirius_event_participant_role_status_tids($participant_role_tid);

	$form['participant_status_tid'] = array(
		'#title' => 'Status',
		'#type' => 'select',
		'#options' => $statuses,
		'#required' => TRUE,
	);

	$form['contact_str'] = array(
		'#title' => t('Contact'),
		'#type' => 'textfield',
		'#required' => TRUE,
    '#autocomplete_path' => 'sirius/event/contact/search/' . $event_node->nid . '/' . $participant_role_tid,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Register',
	);

	return $form;
}

function sirius_node_tab_event_participants_register_byname_form_submit($form, &$form_state) {
	$event_node = $form_state['event_node'];
	$participant_role_tid = $form_state['participant_role_tid'];

	$participant_status_tid = $form_state['values']['participant_status_tid'];
	$str = $form_state['values']['contact_str'];
	$matches = array();
	if (preg_match('/\s+\((\d+)\)/', $str, $matches) && $matches[1]) {
		$contact_nid = $matches[1];
	}
	
	if (!$contact_nid) {
		drupal_set_message(t('No contact selected'), 'warning');
		return;
	}

	$result = sirius_event_participant_register($event_node->nid, $contact_nid, $participant_role_tid, $participant_status_tid);
	if ($result['success']) { drupal_set_message($result['msg']); } else { drupal_set_message($result['msg'], 'error'); }
}
