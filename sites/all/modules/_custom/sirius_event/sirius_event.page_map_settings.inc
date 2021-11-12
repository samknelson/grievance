<?php

function sirius_node_tab_event_map_settings_access($event_node) {
	if (!sirius_node_tab_event_map_access($event_node)) { return FALSE; } 
	
	if (user_access('sirius admin') || user_access('sirius domain admin') || user_access('sirius event admin')) { return TRUE; }

	$participant_nid = sirius_event_participant_nid_from_user($event_node->nid);
	if (!$participant_nid) { return FALSE; }
	$participant_node = node_load($participant_nid);
	$type_json = sirius_event_type_json($event_node);
	if (!$type_json['map']['admin_prole_tids']) { return FALSE; }
	$prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid'];
	$admin_prole_tids = array_values($type_json['map']['admin_prole_tids']);
	if (!in_array($prole_tid, $admin_prole_tids)) { return FALSE; }

	return TRUE;
}

function sirius_node_tab_event_map_settings($event_node) {
  sirius_event_disclaimer_redirect($event_node);
	return drupal_get_form('sirius_node_tab_event_map_settings_form', $event_node->nid);
}

function sirius_node_tab_event_map_settings_form($form, &$form_state, $event_nid) {
	$form_state['event_nid'] = $event_nid;
	$event_node = node_load($event_nid);
	$json = json_decode($event_node->field_sirius_json['und'][0]['value'], TRUE);

	$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
	$prole_tids = sirius_fieldvals($event_type_term, 'field_sirius_event_proles', 'tid');
	$pstatus_options = sirius_event_participant_role_status_tids($prole_tids, array('phonebank' => array('during_call' => TRUE)));

	$form['#tree'] = TRUE;

	$form['map']['active'] = array(
		'#type' => 'select',
		'#title' => t('Active?'),
		'#options' => array(
			'No' => t('No'), 
			'Yes' => t('Yes'),
			'Auto' => t('Auto'),
		),
		'#default_value' => $json['map']['active'],
		'#description' => t('Is the map active -- i.e. is it possible to make calls? Select "Auto" to have the map automatically open and close at the scheduled time.'),
	);

	$form['map']['active_grace_open'] = array(
		'#type' => 'textfield',
		'#size' => 5,
		'#title' => t('Grace Period - Open'),
		'#description' => t('Enter the grace period for opening the phone bank. Calls can be made this many minutes before the scheduled open time.'),
    '#states' => array(
      'visible' => array(
        array(':input[name="map[active]"]' => array('value' => 'Auto')),
      )
    ),
		'#default_value' => $json['map']['active_grace_open'],
	);

	$form['map']['active_grace_close'] = array(
		'#type' => 'textfield',
		'#size' => 5,
		'#title' => t('Grace Period - Close'),
		'#description' => t('Enter the grace period for closing the phone bank. Calls can be made this many minutes after the scheduled close time.'),
    '#states' => array(
      'visible' => array(
        array(':input[name="map[active]"]' => array('value' => 'Auto')),
      )
    ),
		'#default_value' => $json['map']['active_grace_close'],
	);

	if (sirius_domain_component_enabled('mapping:checkin')) {
		$form['map']['checkin']['active'] = array(
			'#type' => 'select',
			'#title' => t('Checkin Active?'),
			'#options' => array(
				'No' => t('No'), 
				'Yes' => t('Yes'),
			),
			'#default_value' => $json['map']['checkin']['active'],
			'#description' => t('Set to "Yes" to enable the "Checkin" feature. This will cause the locations of all map users to be reported back to the server, and will allow administrators to see the locations of other users as they move around the map.'),
		);
	}

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

	$form['map']['group_by_address_options'] = array(
		'#type' => 'fieldset',
		'#title' => t('Group by Address Options'),
    '#states' => array('visible' => array(":input[name=\"map[group_by_address]\"]" => array('value' => 'Yes'))),
	);

	$form['map']['group_by_address_options']['mixed_pstatus'] = array(
		'#type' => 'select',
		'#title' => t('Show Mixed As'),
		'#options' => array(NULL => t('-- Select --')) + $pstatus_options,
		'#default_value' => $json['map']['group_by_address_options']['mixed_pstatus'],
		'#description' => 'When participants at a single address have different statuses, show this pin.',
	);

	$form['map']['caller_intro'] = array(
		'#type' => 'text_format',
		'#title' => 'Introduction for callers',
		'#default_value' => $json['map']['caller_intro'],
		'#description' => t('Callers will receive this message on the landing page of the map.'),
		'#format' => 'sirius_form_editor',
		'#allowed_formats' => array('sirius_form_editor'),
	);

	$form['map']['do_not_disturb'] = array(
		'#type' => 'fieldset',
		'#title' => 'Do Not Disturb',
	);
	$options = array(NULL => '-- Select --') + sirius_do_not_disturb_list();
	$form['map']['do_not_disturb']['id'] = array(
		'#type' => 'select',
		'#options' => $options,
		'#title' => t('Which Do Not Disturb schedule should be used?'),
		'#default_value' => $json['map']['do_not_disturb']['id'],
	);

	if ($json['map']['do_not_disturb']['id']) {
		$form['map']['do_not_disturb']['render'] = array('#markup' => sirius_do_not_disurb_render($json['map']['do_not_disturb']['id']));
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_event_map_settings_form_submit($form, &$form_state) {
	$event_nid = $form_state['event_nid'];
	$event_node = node_load($event_nid);
	$json = sirius_jsonfield_util_node_json_get($event_node);
	$json['map'] = $form_state['values']['map'];
	$json['map']['caller_intro'] = sirius_richtext_handle_input($form_state['values']['map']['caller_intro']);
	sirius_jsonfield_util_node_json_set($event_node, $json);
	node_save($event_node);
	drupal_set_message(t('Settings saved.'));
}