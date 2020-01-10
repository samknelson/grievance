<?php
function sirius_node_tab_event_phonebank_settings_access($event_node) {
	if (!sirius_node_tab_event_phonebank_access($event_node)) { return FALSE; } 

	if (user_access('sirius admin') || user_access('sirius event admin')) { return TRUE; }

	$participant_nid = sirius_event_participant_nid_from_user($event_node->nid);
	if (!$participant_nid) { return FALSE; }
	$participant_node = node_load($participant_nid);
	$type_json = sirius_event_type_json($event_node);
	$prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid'];
	$admin_prole_tids = array_values($type_json['phonebank']['admin_prole_tids']);
	if (!in_array($prole_tid, $admin_prole_tids)) { return FALSE; }

	return TRUE;
}

function sirius_node_tab_event_phonebank_settings($event_node) {
	return drupal_get_form('sirius_node_tab_event_phonebank_settings_form', $event_node->nid);
}

function sirius_node_tab_event_phonebank_settings_form($form, &$form_state, $event_nid) {
	$form_state['event_nid'] = $event_nid;
	$event_node = node_load($event_nid);
	$json = json_decode($event_node->field_sirius_json['und'][0]['value'], TRUE);

	$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
	$prole_tids = sirius_fieldvals($event_type_term, 'field_sirius_event_proles', 'tid');

	$form['active'] = array(
		'#type' => 'select',
		'#title' => t('Active?'),
		'#options' => array('No' => t('No'), 'Yes' => t('Yes')),
		'#default_value' => $json['phonebank']['active'],
		'#description' => t('Is the phonebank active -- i.e. is it possible to make calls?'),
	);

	$pstatus_options = sirius_event_participant_role_status_tids($prole_tids);
	$form['pstatus_tid'] = array(
		'#type' => 'select',
		'#title' => t('Status to Call'),
		'#options' => $pstatus_options,
		'#default_value' => $json['phonebank']['pstatus_tid'],
		'#description' => t('Phone calls will be made to participants with this status.'),
	);

	$form['caller_intro'] = array(
		'#type' => 'textarea',
		'#title' => 'Introduction for callers',
		'#default_value' => $json['phonebank']['caller_intro'],
		'#description' => t('Callers will receive this message on the landing page of the phonebank.'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_event_phonebank_settings_form_submit($form, &$form_state) {
	$event_nid = $form_state['event_nid'];
	$event_node = node_load($event_nid);
	$json = json_decode($event_node->field_sirius_json['und'][0]['value'], TRUE);
	$json['phonebank'] = $form_state['values'];
	$event_node->field_sirius_json['und'][0]['value'] = json_encode($json);
	node_save($event_node);
	drupal_set_message(t('Settings saved.'));

}