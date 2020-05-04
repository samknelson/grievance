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

	$form['#tree'] = TRUE;

	$form['phonebank']['active'] = array(
		'#type' => 'select',
		'#title' => t('Active?'),
		'#options' => array(
			'No' => t('No'), 
			'Yes' => t('Yes'),
			'Auto' => t('Auto'),
		),
		'#default_value' => $json['phonebank']['active'],
		'#description' => t('Is the phonebank active -- i.e. is it possible to make calls? Select "Auto" to have the phonebank automatically open and close at the scheduled time.'),
	);

	$form['phonebank']['active_grace_open'] = array(
		'#type' => 'textfield',
		'#size' => 5,
		'#title' => t('Grace Period - Open'),
		'#description' => t('Enter the grace period for opening the phone bank. Calls can be made this many minutes before the scheduled open time.'),
    '#states' => array(
      'visible' => array(
        array(':input[name="phonebank[active]"]' => array('value' => 'Auto')),
      )
    ),
		'#default_value' => $json['phonebank']['active_grace_open'],
	);

	$form['phonebank']['active_grace_close'] = array(
		'#type' => 'textfield',
		'#size' => 5,
		'#title' => t('Grace Period - Close'),
		'#description' => t('Enter the grace period for closing the phone bank. Calls can be made this many minutes after the scheduled close time.'),
    '#states' => array(
      'visible' => array(
        array(':input[name="phonebank[active]"]' => array('value' => 'Auto')),
      )
    ),
		'#default_value' => $json['phonebank']['active_grace_close'],
	);

	$pstatus_options = sirius_event_participant_role_status_tids($prole_tids);
	$form['phonebank']['pstatus_tid'] = array(
		'#type' => 'select',
		'#title' => t('Status to Call'),
		'#options' => $pstatus_options,
		'#default_value' => $json['phonebank']['pstatus_tid'],
		'#description' => t('Phone calls will be made to participants with this status.'),
	);

	$form['phonebank']['caller_intro'] = array(
		'#type' => 'textarea',
		'#title' => 'Introduction for callers',
		'#default_value' => $json['phonebank']['caller_intro'],
		'#description' => t('Callers will receive this message on the landing page of the phonebank.'),
	);

	$form['phonebank']['conference'] = array(
		'#type' => 'select',
		'#title' => t('Conference Connect?'),
		'#options' => array(
			'No' => t('No'), 
			'Yes' => t('Yes'),
		),
		'#default_value' => $json['phonebank']['conference'],
		'#description' => t('Do you want to set a "conference connect" number for this phonebank?'),
	);

	$form['phonebank']['conference_number'] = array(
		'#type' => 'textfield',
		'#title' => t('Conference Connect Number'),
		'#description' => t('Enter the phone number that volunteers can use for a conference connect'),
    '#states' => array(
      'visible' => array(
        array(':input[name="phonebank[conference]"]' => array('value' => 'Yes')),
      )
    ),
		'#default_value' => $json['phonebank']['conference_number'],
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
	$json = sirius_jsonfield_util_node_json_get($event_node);
	$json['phonebank'] = $form_state['values']['phonebank'];
	sirius_jsonfield_util_node_json_set($event_node, $json);
	node_save($event_node);
	drupal_set_message(t('Settings saved.'));
}