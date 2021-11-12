<?php

function sirius_event_term_tab_map_access($event_type_term) {
	if (!sirius_domain_component_enabled('mapping')) { return FALSE; }
	return TRUE;
}

function sirius_event_term_tab_map($event_type_term) {
	return drupal_get_form('sirius_event_term_tab_map_form', $event_type_term);
}

function sirius_event_term_tab_map_form($form, &$form_state, $event_type_term) {
	$form_state['event_type_tid'] = $event_type_term->tid;
	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);

	$prole_tids = sirius_fieldvals($event_type_term, 'field_sirius_event_proles', 'tid');
	$prole_terms = taxonomy_term_load_multiple($prole_tids);
	$prole_options = array();
	foreach ($prole_terms as $term) {
		$prole_options[$term->tid] = $term->name;
	}
	$prole_tids = array_keys($prole_terms);
	$pstatus_options = array(NULL => t('-- Select --')) + sirius_event_participant_role_status_tids($prole_tids);

	$form['#tree'] = TRUE;
	$form['map_enabled'] = array(
		'#type' => 'select',
		'#title' => t('Map Enabled?'),
		'#options' => array('No' => t('No'), 'Yes' => t('Yes')),
		'#description' => t('Is this event type a map?'),
		'#default_value' => $json['map']['map_enabled'],
	);

	$form['prole_tid'] = array(
		'#type' => 'select',
		'#title' => t('Map Recipient Role'),
		'#options' => $prole_options,
		'#default_value' => $json['map']['prole_tid'],
		'#description' => t('Phone calls will be made to participants with this role.'),
	);

	$form['caller_prole_tids'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Map User Roles'),
		'#options' => $prole_options,
		'#description' => t('Phone calls will be made from participants with this role.'),
	);
	if ($json['map']['caller_prole_tids']) { $form['caller_prole_tids']['#default_value'] = $json['map']['caller_prole_tids']; }

	$form['admin_prole_tids'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Admin Roles'),
		'#options' => $prole_options,
		'#description' => t('These roles have permission to administer the map.'),
	);
	if ($json['map']['admin_prole_tids']) { $form['admin_prole_tids']['#default_value'] = $json['map']['admin_prole_tids']; }

	$form['map']['group_by_address'] = array(
		'#type' => 'select',
		'#title' => t('Group by Address?'),
		'#options' => array(
			'No' => t('No'), 
			'Yes' => t('Yes'),
		),
		'#default_value' => $json['map']['group_by_address'],
		'#description' => t('Set to "Yes" to group pins on the map by address, so that multiple participants at a single address will appear as a single pin.'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_event_term_tab_map_form_submit($form, &$form_state) {
	$event_type_tid = $form_state['event_type_tid'];
	$event_type_term = taxonomy_term_load($event_type_tid);
	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
	$json['map'] = $form_state['values'];
	$event_type_term->field_sirius_json['und'][0]['value'] = json_encode($json);
	taxonomy_term_save($event_type_term);
	drupal_set_message("Settings saved.");
}

