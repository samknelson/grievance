<?php

function sirius_node_tab_event_intake_status_access($event_node) {
	// If it's disabled at the event type level, not enabled
	if (!sirius_event_intake_enabled_tid($event_node->field_sirius_event_type['und'][0]['tid'])) { return FALSE; }

	// Admin user roles can access
	if (user_access('sirius admin') || user_access('sirius domain admin') || user_access('sirius event admin')) { return TRUE; }

	// Admin participant roles can access
	$participant_nid = sirius_event_participant_nid_from_user($event_node->nid);
	if (!$participant_nid) { return FALSE; }
	$participant_node = node_load($participant_nid);
	$type_json = sirius_event_type_json($event_node);
	$event_json = sirius_json_get($event_node);
	$prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid'];
	$admin_prole_tids = array_values($type_json['intake']['admin_prole_tids']);
	if (in_array($prole_tid, $admin_prole_tids)) { return TRUE; }

	// Oh well
	return FALSE;
}

function sirius_node_tab_event_intake_status($event_node) {
  sirius_event_disclaimer_redirect($event_node);
	return drupal_get_form('sirius_node_tab_event_intake_status_form', $event_node);
}

function sirius_node_tab_event_intake_status_form($form, &$form_state, $event_node) {
	$form_state['event_nid'] = $event_node->nid;
	$json = sirius_json_get($event_node);

	$current_status = sirius_event_intake_active($event_node);
	if ($current_status['success']) {
		$current_status_msg = 'Open';
	} else {
		$current_status_msg = 'Closed: ' . $current_status['msg'];
	}
	$form['intro'] = array('#markup' => '<div class="sirius_form_intro">' . 'Current status: <strong>' . $current_status_msg . '</strong></div>');

	$form['#tree'] = TRUE;
	$form['intake_status']['active'] = array(
		'#type' => 'select',
		'#title' => t('Open?'),
		'#options' => array(
			'No' => t('No'), 
			'Yes' => t('Yes'),
			'Auto' => t('Auto'),
		),
		'#default_value' => $json['intake_status']['active'],
		'#description' => t('Is intake open? Select "Auto" to have intakes automatically open and close at the scheduled time.'),
	);

	$form['intake_status']['active_grace_open'] = array(
		'#type' => 'textfield',
		'#size' => 5,
		'#title' => t('Grace Period - Open'),
		'#description' => t('Enter the grace period for opening. Applications can be submitted this many minutes before the scheduled open time.'),
    '#states' => array(
      'visible' => array(
        array(':input[name="intake_status[active]"]' => array('value' => 'Auto')),
      )
    ),
		'#default_value' => $json['intake_status']['active_grace_open'],
	);

	$form['intake_status']['active_grace_close'] = array(
		'#type' => 'textfield',
		'#size' => 5,
		'#title' => t('Grace Period - Close'),
		'#description' => t('Enter the grace period for closing. Applications can be submitted this many minutes after the scheduled close time.'),
    '#states' => array(
      'visible' => array(
        array(':input[name="intake_status[active]"]' => array('value' => 'Auto')),
      )
    ),
		'#default_value' => $json['intake_status']['active_grace_close'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_event_intake_status_form_submit($form, &$form_state) {
	$event_node = node_load($form_state['event_nid']);
	$event_json = sirius_json_get($event_node);
	$event_json['intake_status'] = $form_state['values']['intake_status'];
	sirius_json_set($event_node, $event_json);
	node_save($event_node);
	drupal_set_message("Settings saved.");
}