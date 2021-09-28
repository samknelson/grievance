<?php

function sirius_event_page_create_access($event_type_tid) {
	// Admins can create
  if (user_access('sirius event admin')) { return TRUE; }

  // Check our creation rules
  $event_type_term = taxonomy_term_load($event_type_tid);
  $event_type_json = sirius_jsonfield_util_term_json_get($event_type_term);
	$rule = sirius_event_paccess_rule_match(NULL, $event_type_json['create']['rules']);
	if ($rule) { return TRUE; }

  // Oh well
  return FALSE;
}

function sirius_event_page_create($event_type_tid) {
	$event_type_term = taxonomy_term_load($event_type_tid);
	drupal_set_title(t('Create an event: @name', array('@name' => $event_type_term->name)));
	return drupal_get_form('sirius_event_page_create_form', $event_type_tid);
}

function sirius_event_page_create_form($form, &$form_state, $event_type_tid) {
	$form_state['event_type_tid'] = $event_type_tid;
	$event_type_term = taxonomy_term_load($event_type_tid);

	$form['title'] = array(
		'#title' => t('Event Title'),
		'#type' => 'textfield',
		'#required' => TRUE,
		'#default_value' => $event_type_term->name,
	);

  $form["start_date"] = array(
    '#title' => t('Date'),
		'#type' => 'date_popup',
		'#timepicker' => 'timepicker',
		'#timepicker_options' => array(
			'rows' => 6,
			'minutes' => array(
			  'starts' => 0,
			  'ends' => 56,
			  'interval' => 5,
			),
		),
		'#attributes' => array(
			'autocomplete' => 'off',
		),
		'#required' => TRUE,
  );
  $form["duration"] = array(
    '#title' => t('Duration'),
		'#type' => 'select',
		'#options' => sirius_event_duration_options($event_type_term),
		'#required' => TRUE,
  );

  $form["description"] = array(
    '#title' => t('Description'),
		'#type' => 'text_format',
		'#required' => TRUE,
		'#default_value' => $event_type_term->description,
  );

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Create a new @name', array('@name' => $event_type_term->name)),
	);

	return $form;
}

function sirius_event_page_create_form_validate($form, &$form_state) {
	$ts = strtotime($form_state['values']["start_date"]);
	if ($ts < time()) { form_set_error('start_date', t('Please choose a date in the future.')); }
}

function sirius_event_page_create_form_submit($form, &$form_state) {
	$event_type_tid = $form_state['event_type_tid'];
	$event_type_term = taxonomy_term_load($event_type_tid);
	global $user;
	$domain_nid = sirius_domain_nid();

  $event_type_term = taxonomy_term_load($event_type_tid);
  $event_type_json = sirius_jsonfield_util_term_json_get($event_type_term);

	// Create the event node
	$event_node = new stdClass();
	$event_node->type = 'sirius_event';
	$event_node->no_recurse = TRUE;
	$event_node->language = $user->language;
	$event_node->uid = $user->uid;
	$event_node->status = 1;
	$event_node->promote = 0;
	$event_node->comment = 0;
	$event_node->title = $form_state['values']['title'];
	$event_node->field_sirius_event_type['und'][0]['tid'] = $event_type_tid;
	$ts = strtotime($form_state['values']["start_date"]);
	$ts2 = $ts + $form_state['values']["duration"];
	if ($ts2 <= 0) { $ts2 = $ts; }
	$event_node->field_sirius_daterepeat['und'][0]['value'] = gmdate('Y-m-d H:i:s', $ts);
	$event_node->field_sirius_daterepeat['und'][0]['value2'] = gmdate('Y-m-d H:i:s', $ts2);
	if ($domain_nid) { $event_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; }
	$event_node->body['und'][0]['value'] = $form_state['values']['description']['value'];
	$event_node->body['und'][0]['format'] = $form_state['values']['description']['format'];
	$event_node->field_sirius_active['und'][0]['value'] = 'Yes';

	// Some things get set at the type level and are copied into the event.
	$event_json = array();
	foreach (array('disclaimer', 'section', 'customfield', 'intake', 'map') as $key) {
		if ($event_type_json[$key]) { 
			$event_json[$key] = $event_type_json[$key];
		}
	}
	if ($event_json) {
		sirius_jsonfield_util_node_json_set($event_node, $event_json);
	}
	node_save($event_node);
	drupal_set_message(t("Created a new event: ") . $event_type_term->name);

	// Register the user
  $rules = $event_type_json['create']['rules'];
	$rule = sirius_event_paccess_rule_match(NULL, $event_type_json['create']['rules']);
	if ($rule['register_as']) {
		list ($prole, $pstatus) = explode('|', $rule['register_as']);
		$result = sirius_event_participant_register_user($event_node->nid, $user->uid, $prole, $pstatus);
		if ($result['success']) { drupal_set_message("Registered the user as a participant."); } 
		else { drupal_set_message(t('Registration failed: ') . $result['msg'], 'error'); }
	}

	drupal_goto('node/' . $event_node->nid . '/sirius_event_info');
}
