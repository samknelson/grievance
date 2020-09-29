<?php

function sirius_event_term_tab_phonebank($event_type_term) {
	return drupal_get_form('sirius_event_term_tab_phonebank_form', $event_type_term);
}

function sirius_event_term_tab_phonebank_form($form, &$form_state, $event_type_term) {
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
	$form['phonebank_enabled'] = array(
		'#type' => 'select',
		'#title' => t('Phonebank Enabled?'),
		'#options' => array('No' => t('No'), 'Yes' => t('Yes')),
		'#description' => t('Is this event type a phonebank?'),
		'#default_value' => $json['phonebank']['phonebank_enabled'],
	);

	$form['prole_tid'] = array(
		'#type' => 'select',
		'#title' => t('Call Recipient Role'),
		'#options' => $prole_options,
		'#default_value' => $json['phonebank']['prole_tid'],
		'#description' => t('Phone calls will be made to participants with this role.'),
	);

	$form['caller_prole_tids'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Caller Roles'),
		'#options' => $prole_options,
		'#description' => t('Phone calls will be made from participants with this role.'),
	);
	if ($json['phonebank']['caller_prole_tids']) { $form['caller_prole_tids']['#default_value'] = $json['phonebank']['caller_prole_tids']; }

	$form['admin_prole_tids'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Admin Roles'),
		'#options' => $prole_options,
		'#description' => t('These roles have permission to administer the phonebank.'),
	);
	if ($json['phonebank']['admin_prole_tids']) { $form['admin_prole_tids']['#default_value'] = $json['phonebank']['admin_prole_tids']; }

	$form['prescan']['pstatus_tid'] = array(
		'#type' => 'select',
		'#title' => t('Pre-Scan Default Status'),
		'#options' => $pstatus_options,
		'#description' => t('When a pre-scan for invalid phone numbers is performed, use this as the default status when an invalid number is found.'),
		'#required' => TRUE,
	);
	if ($json['phonebank']['prescan']['pstatus_tid']) { $form['prescan']['pstatus_tid']['#default_value'] = $json['phonebank']['prescan']['pstatus_tid']; }

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_event_term_tab_phonebank_form_submit($form, &$form_state) {
	$event_type_tid = $form_state['event_type_tid'];
	$event_type_term = taxonomy_term_load($event_type_tid);
	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
	$json['phonebank'] = $form_state['values'];
	$event_type_term->field_sirius_json['und'][0]['value'] = json_encode($json);
	taxonomy_term_save($event_type_term);
	drupal_set_message("Settings saved.");
}

